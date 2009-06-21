 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca/application/decodercomponent.h>

BoCA::AS::DecoderComponent::DecoderComponent(ComponentSpecs *iSpecs) : Component(iSpecs)
{
}

BoCA::AS::DecoderComponent::~DecoderComponent()
{
}

Int BoCA::AS::DecoderComponent::GetPackageSize()
{
	return specs->func_GetPackageSize(component);
}

Int BoCA::AS::DecoderComponent::SetDriver(IO::Driver *driver)
{
	return specs->func_SetDriver(component, driver);
}

Bool BoCA::AS::DecoderComponent::SetAudioTrackInfo(const Track &track)
{
	return specs->func_SetAudioTrackInfo(component, &track);
}

Bool BoCA::AS::DecoderComponent::CanOpenStream(const String &file)
{
	return specs->func_CanOpenStream(component, file);
}

Error BoCA::AS::DecoderComponent::GetStreamInfo(const String &file, Track &track)
{
	return specs->func_GetStreamInfo(component, file, &track);
}

Error BoCA::AS::DecoderComponent::UpdateStreamInfo(const String &file, const Track &track)
{
	return specs->func_UpdateStreamInfo(component, file, &track);
}

Bool BoCA::AS::DecoderComponent::Activate()
{
	SetDriver(driver);

	if (specs->func_Activate(component))
	{
		packageSize = GetPackageSize();

		return True;
	}

	return False;
}

Bool BoCA::AS::DecoderComponent::Deactivate()
{
	return specs->func_Deactivate(component);
}

Int BoCA::AS::DecoderComponent::ReadData(Buffer<UnsignedByte> &buffer, Int size)
{
	return specs->func_ReadData(component, &buffer, size);
}

Int64 BoCA::AS::DecoderComponent::GetInBytes()
{
	return specs->func_GetInBytes(component);
}
