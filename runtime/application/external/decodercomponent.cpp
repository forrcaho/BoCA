 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca/application/external/decodercomponent.h>
#include <boca/application/external/configlayer.h>

#include <boca/application/registry.h>
#include <boca/application/taggercomponent.h>

#include <boca/common/utilities.h>

BoCA::AS::DecoderComponentExternal::DecoderComponentExternal(ComponentSpecs *specs) : DecoderComponent(specs)
{
	configuration	= NIL;

	configLayer	= NIL;

	packageSize	= 0;
	inBytes		= 0;
}

BoCA::AS::DecoderComponentExternal::~DecoderComponentExternal()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);
}

Bool BoCA::AS::DecoderComponentExternal::CanOpenStream(const String &streamURI)
{
	String	 lcURI = streamURI.ToLower();

	for (Int i = 0; i < specs->formats.Length(); i++)
	{
		FileFormat	*format = specs->formats.GetNth(i);

		for (Int j = 0; j < format->GetExtensions().Length(); j++)
		{
			if (lcURI.EndsWith(String(".").Append(format->GetExtensions().GetNth(j)))) return True;
		}
	}

	return False;
}

Bool BoCA::AS::DecoderComponentExternal::SetAudioTrackInfo(const Track &track)
{
	this->track = track;

	return True;
}

Int BoCA::AS::DecoderComponentExternal::SetDriver(IO::Driver *driver)
{
	return IO::Filter::SetDriver(driver);
}

Int BoCA::AS::DecoderComponentExternal::ProcessData(Buffer<UnsignedByte> &buffer)
{
	/* Find system byte order.
	 */
	static Int	 systemByteOrder = CPU().GetEndianness() == EndianLittle ? BYTE_INTEL : BYTE_RAW;

	/* Switch byte order to native.
	 */
	const Format	&format = track.GetFormat();

	if (format.order != BYTE_NATIVE && format.order != systemByteOrder) Utilities::SwitchBufferByteOrder(buffer, format.bits / 8);

	/* Calculate MD5 if requested.
	 */
	if (calculateMD5) md5.Feed(buffer);

	return buffer.Size();
}

BoCA::ConfigLayer *BoCA::AS::DecoderComponentExternal::GetConfigurationLayer()
{
	if (configLayer == NIL && specs->parameters.Length() > 0) configLayer = new ConfigLayerExternal(specs);

	return configLayer;
}

const BoCA::Config *BoCA::AS::DecoderComponentExternal::GetConfiguration() const
{
	if (configuration != NIL) return configuration;
	else			  return Config::Get();
}

Bool BoCA::AS::DecoderComponentExternal::SetConfiguration(const Config *nConfiguration)
{
	configuration = nConfiguration;

	return True;
}

Int BoCA::AS::DecoderComponentExternal::QueryTags(const String &streamURI, Track &track) const
{
	/* Loop over supported formats.
	 */
	String	 lcURI = streamURI.ToLower();

	foreach (FileFormat *format, specs->formats)
	{
		foreach (const String &extension, format->GetExtensions())
		{
			if (!lcURI.EndsWith(String(".").Append(extension))) continue;

			/* Read supported tag formats.
			 */
			const Array<TagFormat>	&tagFormats = format->GetTagFormats();

			foreach (const TagFormat &tagFormat, tagFormats)
			{
				AS::Registry		&boca	= AS::Registry::Get();
				AS::TaggerComponent	*tagger = (AS::TaggerComponent *) boca.CreateComponentByID(tagFormat.GetTagger());

				if (tagger != NIL)
				{
					tagger->SetConfiguration(GetConfiguration());
					tagger->ParseStreamInfo(streamURI, track);

					boca.DeleteComponent(tagger);
				}
			}

			/* Set lossless flag for chapters.
			 */
			if (track.lossless) foreach (Track &chapter, track.tracks) chapter.lossless = True;

			break;
		}
	}

	return Success();
}
