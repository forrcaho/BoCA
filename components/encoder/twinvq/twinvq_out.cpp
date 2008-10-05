 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>

using namespace smooth::IO;

#include "twinvq_out.h"
#include "config.h"
#include "dllinterface.h"

#include "twinvq/bfile_e.h"
#include "twinvq/bstream_e.h"
#include "twinvq/Chunk.h"
#include "twinvq/ChunkHelper.h"

const String &BoCA::TwinVQOut::GetComponentSpecs()
{
	static String	 componentSpecs;

	if (twinvqdll != NIL)
	{
		componentSpecs = "				\
								\
		  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
		  <component>					\
		    <name>TwinVQ VQF Encoder</name>		\
		    <version>1.0</version>			\
		    <id>twinvq-out</id>				\
		    <type>encoder</type>			\
		    <format>					\
		      <name>TwinVQ VQF Audio</name>		\
		      <extension>vqf</extension>		\
		    </format>					\
		  </component>					\
								\
		";
	}

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
	LoadTwinVQDLL();
}

Void smooth::DetachDLL()
{
	FreeTwinVQDLL();
}

BoCA::TwinVQOut::TwinVQOut()
{
	configLayer = NIL;
}

BoCA::TwinVQOut::~TwinVQOut()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);
}

Bool BoCA::TwinVQOut::Activate()
{
	Config	*config = Config::Get();

	const Format	&format = track.GetFormat();

	switch (format.rate)
	{
		case 22050:
			if (config->GetIntValue("TwinVQ", "Bitrate", 48) == 48)
			{
				Utilities::ErrorMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.");

				errorState = True;

				return False;
			}
			break;
		case 44100:
			if (config->GetIntValue("TwinVQ", "Bitrate", 48) != 48)
			{
				Utilities::ErrorMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.");

				errorState = True;

				return False;
			}
			break;
		default:
			Utilities::ErrorMessage("Bad sampling rate! The selected sampling rate is not supported.");

			errorState = True;

			return False;
	}

	if (format.channels > 2)
	{
		Utilities::ErrorMessage("BonkEnc does not support more than 2 channels!");

		errorState = True;

		return False;
	}

	memset(&setupInfo, 0, sizeof(headerInfo));
	memset(&encInfo, 0, sizeof(encSpecificInfo));
	
	ex_TvqGetVersionID(V2, setupInfo.ID);

	setupInfo.channelMode = format.channels - 1;
	setupInfo.samplingRate = int(format.rate / 1000);
	setupInfo.bitRate = config->GetIntValue("TwinVQ", "Bitrate", 48) * format.channels;

	if (track.artist != NIL || track.title != NIL)
	{
		Config	*currentConfig = Config::Get();

		if	(track.artist != NIL) strncpy(setupInfo.Auth, track.artist, Math::Min(track.artist.Length(), 1024));
		if	(track.title  != NIL) strncpy(setupInfo.Name, track.title,  Math::Min(track.title.Length(),  1024));

		if	(track.comment != NIL && !config->replace_comments) strncpy(setupInfo.Comt, track.comment,		    Math::Min(track.comment.Length(),		       1024));
		else if (currentConfig->default_comment != NIL)		    strncpy(setupInfo.Comt, currentConfig->default_comment, Math::Min(currentConfig->default_comment.Length(), 1024));
	}

	encInfo.N_CAN_GLOBAL = config->GetIntValue("TwinVQ", "PreselectionCandidates", 32); // number of VQ pre-selection candidates

	ex_TvqEncInitialize(&setupInfo, &encInfo, &index, 0);

	unsigned long	 samples_size = ex_TvqEncGetFrameSize() * ex_TvqEncGetNumChannels();

	packageSize = samples_size * (format.bits / 8);

	frame.Resize(samples_size);

	TvqInitBsWriter();

	CChunkChunk	*twinChunk	= TvqCreateHeaderChunk(&setupInfo, "header_info");

	bfp = bopen(Utilities::GetNonUnicodeTempFileName(track.outfile).Append(".out"), (char *) "wb");

	TvqPutBsHeaderInfo(bfp, *twinChunk);

	delete twinChunk;

	return True;
}

Bool BoCA::TwinVQOut::Deactivate()
{
	/* Flush TwinVQ buffers by writing two empty frames.
	 */
	frame.Zero();

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, bfp);

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, bfp);

	ex_TvqEncTerminate(&index);

	bclose(bfp);

	/* Stream contents of created VQF file to output driver
	 */
	InStream		 in(STREAM_FILE, Utilities::GetNonUnicodeTempFileName(track.outfile).Append(".out"), IS_READONLY);
	Buffer<UnsignedByte>	 buffer(1024);
	Int			 bytesLeft = in.Size();

	while (bytesLeft)
	{
		driver->WriteData((UnsignedByte *) in.InputData(buffer, Math::Min(1024, bytesLeft)), Math::Min(1024, bytesLeft));

		bytesLeft -= Math::Min(1024, bytesLeft);
	}

	in.Close();

	File(Utilities::GetNonUnicodeTempFileName(track.outfile).Append(".out")).Delete();

	return True;
}

Int BoCA::TwinVQOut::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	const Format	&format = track.GetFormat();

	samplesBuffer.Resize(size / (format.bits / 8));

	for (int i = 0; i < size / (format.bits / 8); i++)
	{
		if (format.bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
		if (format.bits == 16)	samplesBuffer[i] = ((short *) (unsigned char *) data)[i];
		if (format.bits == 24)	samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format.bits == 32)	samplesBuffer[i] = (int) ((long *) (unsigned char *) data)[i] / 65536;
	}

	for (int ch = 0; ch < format.channels; ch++)
	{
		for (int i = 0; i < int(size / (format.bits / 8) / format.channels); i++)
		{
			frame[ch * int(frame.Size() / format.channels) + i] = (float) samplesBuffer[i * format.channels + ch];
		}
	}

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, bfp);

	return size;
}

ConfigLayer *BoCA::TwinVQOut::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureTwinVQ();

	return configLayer;
}

Void BoCA::TwinVQOut::FreeConfigurationLayer()
{
	if (configLayer != NIL)
	{
		delete configLayer;

		configLayer = NIL;
	}
}
