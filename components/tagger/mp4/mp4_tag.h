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

BoCA_BEGIN_COMPONENT(MP4Tag)

namespace BoCA
{
	class MP4Tag : public CS::TaggerComponent
	{
		private:
			static const String	 genres[148];

			const String		&GetID3CategoryName(Int);
		public:
			static const String	&GetComponentSpecs();

						 MP4Tag();
						~MP4Tag();

			Error			 ParseStreamInfo(const String &, Track &);

			Error			 RenderStreamInfo(const String &, const Track &);
			Error			 UpdateStreamInfo(const String &, const Track &);
	};
};

BoCA_DEFINE_TAGGER_COMPONENT(MP4Tag)

BoCA_END_COMPONENT(MP4Tag)
