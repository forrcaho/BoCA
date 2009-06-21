 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca/application/external/encodercomponentstdio.h>
#include <boca/common/config.h>
#include <boca/common/utilities.h>

#include <boca/common/tagging/tagid3v1.h>
#include <boca/common/tagging/tagid3v2.h>
#include <boca/common/tagging/tagmp4.h>
#include <boca/common/tagging/tagape.h>

#include <smooth/io/drivers/driver_win32.h>

using namespace smooth::IO;

BoCA::AS::EncoderComponentExternalStdIO::EncoderComponentExternalStdIO(ComponentSpecs *specs) : EncoderComponentExternal(specs)
{
	out = NIL;
}

BoCA::AS::EncoderComponentExternalStdIO::~EncoderComponentExternalStdIO()
{
}

Bool BoCA::AS::EncoderComponentExternalStdIO::Activate()
{
	SECURITY_ATTRIBUTES	 secAttr;

	ZeroMemory(&secAttr, sizeof(secAttr));

	secAttr.nLength		= sizeof(secAttr);
	secAttr.bInheritHandle	= True;

	CreatePipe(&rPipe, &wPipe, &secAttr, 131072);
	SetHandleInformation(wPipe, HANDLE_FLAG_INHERIT, 0);

	/* Start 3rd party command line encoder
	 */
	STARTUPINFOA		 startupInfo;

	ZeroMemory(&startupInfo, sizeof(startupInfo));

	startupInfo.cb		= sizeof(startupInfo);
	startupInfo.dwFlags	= STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow	= specs->debug ? SW_SHOW : SW_HIDE;
	startupInfo.hStdInput	= rPipe;
	startupInfo.hStdOutput	= NIL;
	startupInfo.hStdError	= NIL;

	PROCESS_INFORMATION	 processInfo;

	ZeroMemory(&processInfo, sizeof(processInfo));

	encFileName = Utilities::GetNonUnicodeTempFileName(track.outfile).Append(".").Append(GetOutputFileExtension());

	/* Remove temporary file if it exists.
	 * Might be a leftover of a previous encoding attempt.
	 */
	File(encFileName).Delete();

	const Info	&info = track.GetInfo();

	String	 arguments = String(specs->external_arguments).Replace("%OPTIONS", specs->GetExternalArgumentsString())
							      .Replace("%OUTFILE", String("\"").Append(encFileName).Append("\""))
							      .Replace("%ARTIST", String("\"").Append((char *) info.artist).Append("\""))
							      .Replace("%ALBUM", String("\"").Append((char *) info.album).Append("\""))
							      .Replace("%TITLE", String("\"").Append((char *) info.title).Append("\""))
							      .Replace("%TRACK", String("\"").Append(String::FromInt(info.track)).Append("\""))
							      .Replace("%YEAR", String("\"").Append(String::FromInt(info.year)).Append("\""))
							      .Replace("%GENRE", String("\"").Append((char *) info.genre).Append("\""));

	CreateProcessA(NIL, String(specs->external_command).Append(" ").Append(arguments), NIL, NIL, True, 0, NIL, NIL, &startupInfo, &processInfo);

	hProcess = processInfo.hProcess;

	driver_stdin = new DriverWin32(wPipe);
	out = new OutStream(STREAM_DRIVER, driver_stdin);

	/* Output WAVE header
	 */
	out->OutputString("RIFF");
	out->OutputNumber(36, 4);
	out->OutputString("WAVE");
	out->OutputString("fmt ");

	out->OutputNumber(16, 4);
	out->OutputNumber(1, 2);
	out->OutputNumber(format.channels, 2);
	out->OutputNumber(format.rate, 4);
	out->OutputNumber(format.rate * format.channels * (format.bits / 8), 4);
	out->OutputNumber(format.channels * (format.bits / 8), 2);

	out->OutputNumber(format.bits, 2);
	out->OutputString("data");
	out->OutputNumber(0, 4);

	out->Flush();

	return True;
}

Bool BoCA::AS::EncoderComponentExternalStdIO::Deactivate()
{
	/* Close stdio pipe
	 */
	delete out;
	delete driver_stdin;

	CloseHandle(rPipe);
	CloseHandle(wPipe);

	/* Wait until the encoder exits
	 */
	unsigned long	 exitCode = 0;

	do
	{
		Sleep(100);

		GetExitCodeProcess(hProcess, &exitCode);
	}
	while (exitCode == STILL_ACTIVE);

	if (exitCode != 0) return False;

	Config	*config = Config::Get();

	/* Create tag if requested
	 */
	Buffer<UnsignedByte>	 tag;
	Int			 tagSize = 0;

	const Info		&info = track.GetInfo();

	if (specs->external_tagmode != TAG_MODE_NONE && (info.artist != NIL || info.title != NIL))
	{
		if	(specs->external_tag == "ID3v1"	      && config->GetIntValue("Tags", "EnableID3v1", False)	&& config->enable_id3)	tagSize = TagID3v1().Render(track, tag);
		else if (specs->external_tag == "ID3v2"	      && config->GetIntValue("Tags", "EnableID3v2", True)	&& config->enable_id3)	tagSize = TagID3v2().Render(track, tag);
		else if (specs->external_tag == "MP4Metadata" && config->GetIntValue("Tags", "EnableMP4Metadata", True) && config->enable_mp4)		  TagMP4().Render(track, encFileName);
		else if (specs->external_tag == "APEv2"	      && config->GetIntValue("Tags", "EnableAPEv2", True))				tagSize = TagAPE().Render(track, tag);
	}

	/* Prepend tag
	 */
	if (specs->external_tagmode == TAG_MODE_PREPEND)
	{
		driver->WriteData(tag, tagSize);
	}

	/* Stream contents of created file to output driver
	 */
	InStream		 in(STREAM_FILE, encFileName, IS_READONLY);
	Buffer<UnsignedByte>	 buffer(1024);
	Int			 bytesLeft = in.Size();

	while (bytesLeft)
	{
		driver->WriteData((UnsignedByte *) in.InputData(buffer, Math::Min(1024, bytesLeft)), Math::Min(1024, bytesLeft));

		bytesLeft -= Math::Min(1024, bytesLeft);
	}

	/* Append tag
	 */
	if (specs->external_tagmode == TAG_MODE_APPEND)
	{
		driver->WriteData(tag, tagSize);
	}

	in.Close();

	File(encFileName).Delete();

	return True;
}

Int BoCA::AS::EncoderComponentExternalStdIO::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	/* Check if external encoder still exists.
	 */
	unsigned long	 exitCode = 0;

	GetExitCodeProcess(hProcess, &exitCode);

	if (exitCode != STILL_ACTIVE) return -1;

	/* Hand data over to the encoder using the stdio pipe
	 */
	out->OutputData(data, size);
	out->Flush();

	return size;
}
