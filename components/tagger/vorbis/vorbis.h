 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca.h>

BoCA_BEGIN_COMPONENT(TaggerVorbis)

namespace BoCA
{
	class TaggerVorbis : public CS::TaggerComponent
	{
		private:
			Int			 RenderTagHeader(const String &, Int, Buffer<UnsignedByte> &);
			Int			 RenderTagItem(const String &, const String &, Buffer<UnsignedByte> &);
		public:
			static const String	&GetComponentSpecs();

						 TaggerVorbis();
						~TaggerVorbis();

			Error			 ParseBuffer(const Buffer<UnsignedByte> &, Track &);

			Error			 RenderBuffer(Buffer<UnsignedByte> &, const Track &);
	};
};

BoCA_DEFINE_TAGGER_COMPONENT(TaggerVorbis)

BoCA_END_COMPONENT(TaggerVorbis)
