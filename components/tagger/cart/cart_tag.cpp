 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <string.h>

#include "cart_tag.h"

using namespace smooth::IO;

const String &BoCA::CartTag::GetComponentSpecs()
{
	static String	 componentSpecs = "				\
									\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>			\
	  <component>							\
	    <name>RIFF Cart Tagger</name>				\
	    <version>1.0</version>					\
	    <id>cart-tag</id>						\
	    <type>tagger</type>						\
	    <format>							\
	      <name>Windows Wave Files</name>				\
	      <extension>wav</extension>				\
	    </format>							\
	    <tagformat default=\"false\">				\
	      <name>RIFF Cart Tag</name>				\
	      <encodings>						\
		<encoding default=\"true\">ISO-8859-1</encoding>	\
		<encoding>UTF-8</encoding>				\
	      </encodings>						\
	    </tagformat>						\
	  </component>							\
									\
	";

	return componentSpecs;
}

BoCA::CartTag::CartTag()
{
}

BoCA::CartTag::~CartTag()
{
}

Error BoCA::CartTag::RenderBuffer(Buffer<UnsignedByte> &buffer, const Track &track)
{
	Config		*currentConfig = Config::Get();
	String		 prevOutFormat = String::SetOutputFormat(currentConfig->GetStringValue("Tags", "RIFFCartTagEncoding", "ISO-8859-1"));

	const Info	&info = track.GetInfo();
	const Format	&format = track.GetFormat();

	buffer.Resize(8);

	/* Cart chunk version 1.01.
	 */
	RenderStringItem("0101", 4, buffer);

	/* Artist and title.
	 */
	RenderStringItem(info.title, 64, buffer);
	RenderStringItem(info.artist, 64, buffer);

	/* Cut number.
	 */
	RenderStringItem(NIL, 64, buffer);

	/* Client ID.
	 */
	RenderStringItem(NIL, 64, buffer);

	/* Category.
	 */
	RenderStringItem("MUS", 64, buffer);

	/* Classification.
	 */
	RenderStringItem(NIL, 64, buffer);

	/* Out cue.
	 */
	RenderStringItem(info.album, 64, buffer);

	/* Start date and time.
	 */
	RenderStringItem("1900-01-01", 10, buffer);
	RenderStringItem("00:00:00", 8, buffer);

	/* End date and time.
	 */
	RenderStringItem("9999-12-31", 10, buffer);
	RenderStringItem("23:59:59", 8, buffer);

	/* Application ID and version.
	 */
	RenderStringItem(Application::Get()->getClientName.Call(), 64, buffer);
	RenderStringItem(Application::Get()->getClientVersion.Call(), 64, buffer);

	/* User defined string.
	 */
	RenderStringItem(String(info.genre).Append(info.year > 0 ? String("|").Append(String::FromInt(info.year)) : String()), 64, buffer);

	/* Level reference.
	 */
	RenderIntegerItem(Math::Pow(2, format.bits - 1), 4, buffer);

	/* Timers.
	 */
	for (Int i = 0; i < 7; i++) RenderTimerItem(NIL, 0, buffer);

	if (track.length >= 0)	RenderTimerItem("EOD", track.length, buffer);
	else			RenderTimerItem(NIL, 0, buffer);

	/* Reserved.
	 */
	RenderStringItem(NIL, 276, buffer);

	/* URL.
	 */
	RenderStringItem(NIL, 1024, buffer);

	/* Tag header.
	 */
	RenderTagHeader(buffer);

	String::SetOutputFormat(prevOutFormat);

	return Success();
}

Int BoCA::CartTag::RenderTagHeader(Buffer<UnsignedByte> &buffer)
{
	OutStream	 out(STREAM_BUFFER, buffer, 8);

	out.OutputString("cart");
	out.OutputNumber(buffer.Size() - 8, 4);

	return Success();
}

Int BoCA::CartTag::RenderStringItem(const String &value, Int bufferSize, Buffer<UnsignedByte> &buffer)
{
	buffer.Resize(buffer.Size() + bufferSize);

	OutStream	 out(STREAM_BUFFER, buffer + buffer.Size() - bufferSize, bufferSize);

	Int		 stringSize = (value != NIL) ? strlen(value) : 0;

	out.OutputString(stringSize <= bufferSize ? value : value.Head(bufferSize));

	for (Int i = 0; i < bufferSize - stringSize; i++) out.OutputNumber(0, 1);

	return Success();
}

Int BoCA::CartTag::RenderIntegerItem(Int value, Int bufferSize, Buffer<UnsignedByte> &buffer)
{
	buffer.Resize(buffer.Size() + bufferSize);

	OutStream	 out(STREAM_BUFFER, buffer + buffer.Size() - bufferSize, bufferSize);

	out.OutputNumber(value, bufferSize);

	return Success();
}

Int BoCA::CartTag::RenderTimerItem(const String &fourcc, Int value, Buffer<UnsignedByte> &buffer)
{
	buffer.Resize(buffer.Size() + 8);

	OutStream	 out(STREAM_BUFFER, buffer + buffer.Size() - 8, 8);
	Int		 fourccSize = (fourcc != NIL) ? strlen(fourcc) : 0;

	out.OutputString(fourccSize <= 4 ? fourcc : fourcc.Head(4));

	for (Int i = 0; i < 4 - fourccSize; i++) out.OutputNumber(0, 1);

	out.OutputNumber(value, 4);

	return Success();
}

Error BoCA::CartTag::ParseBuffer(const Buffer<UnsignedByte> &buffer, Track &track)
{
	InStream	 in(STREAM_BUFFER, buffer, buffer.Size());
	Bool		 error = False;

	/* Read cart chunk.
	 */
	if (in.InputString(4) != "cart") error = True;

	/* Skip header.
	 */
	in.RelSeek(4);

	/* Parse individual comment items.
	 */
	Info	 info = track.GetInfo();
	String	 prevInFormat = String::SetInputFormat("ISO-8859-1");

	/* Cart chunk version.
	 */
	String	 version = in.InputString(4);

	/* Continue if version == 1.x.
	 */
	if (version.ToInt() >= 100 && version.ToInt() < 200)
	{
		/* Artist and title.
		 */
		info.title  = in.InputString(64);
		info.artist = in.InputString(64);

		/* Cut number.
		 */
		in.RelSeek(64);

		/* Client ID.
		 */
		in.RelSeek(64);

		/* Category.
		 */
		in.RelSeek(64);

		/* Classification.
		 */
		in.RelSeek(64);

		/* Out cue.
		 */
		in.RelSeek(64);

		/* Start date and time.
		 */
		in.RelSeek(18);

		/* End date and time.
		 */
		in.RelSeek(18);

		/* Application ID and version.
		 */
		in.RelSeek(64);
		in.RelSeek(64);

		/* User defined string.
		 */
		in.RelSeek(64);

		/* Level reference.
		 */
		in.RelSeek(4);

		/* Timers.
		 */
		for (Int i = 0; i < 8; i++) in.RelSeek(8);

		/* Reserved.
		 */
		in.RelSeek(276);

		/* URL.
		 */
		in.RelSeek(1024);
	}

	track.SetInfo(info);

	String::SetInputFormat(prevInFormat);

	if (error) return Error();
	else	   return Success();
}

Error BoCA::CartTag::ParseStreamInfo(const String &fileName, Track &track)
{
	InStream	 in(STREAM_FILE, fileName, IS_READ);
	Bool		 error = False;

	/* Read RIFF chunk.
	 */
	if (in.InputString(4) != "RIFF") error = True;

	in.RelSeek(8);

	String		 chunk;

	while (!error && chunk != "cart")
	{
		if (in.GetPos() >= in.Size()) break;

		/* Read next chunk.
		 */
		chunk = in.InputString(4);

		Int	 cSize = in.InputNumber(4);

		if (chunk == "cart")
		{
			Buffer<UnsignedByte>	 buffer(cSize + 8);

			in.RelSeek(-8);
			in.InputData(buffer, cSize + 8);

			if (ParseBuffer(buffer, track) != Success()) error = True;
		}
		else
		{
			/* Skip chunk.
			 */
			in.RelSeek(cSize);
		}
	}

	if (error) return Error();
	else	   return Success();
}
