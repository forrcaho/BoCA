 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca.h>

#include "dllinterface.h"

BoCA_BEGIN_COMPONENT(SndFileIn)

namespace BoCA
{
	class SndFileIn : public CS::DecoderComponent
	{
		private:
			FILE			*file;
			SNDFILE			*sndf;
		public:
			static const String	&GetComponentSpecs();

						 SndFileIn();
						~SndFileIn();

			Bool			 CanOpenStream(const String &);
			Error			 GetStreamInfo(const String &, Track &);

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);
	};
};

BoCA_DEFINE_DECODER_COMPONENT(SndFileIn)

BoCA_END_COMPONENT(SndFileIn)
