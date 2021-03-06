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

#ifndef H_CDIO_CDINFO
#define H_CDIO_CDINFO

#include <smooth.h>

using namespace smooth;

namespace BoCA
{
	class CDInfo
	{
		private:
			String		 artist;
			String		 title;

			Array<String>	 trackArtists;
			Array<String>	 trackTitles;
		public:
					 CDInfo();
					~CDInfo();

			Int		 Clear();
		accessors:
			Void		 SetArtist(const String &nArtist)	{ artist = nArtist; }
			const String	&GetArtist() const			{ return artist; }

			Void		 SetTitle(const String &nTitle)		{ title = nTitle; }
			const String	&GetTitle() const			{ return title; }

			Void		 SetTrackArtist(Int n, const String &);
			const String	&GetTrackArtist(Int n) const		{ return trackArtists.GetNth(n - 1); }

			Void		 SetTrackTitle(Int n, const String &);
			const String	&GetTrackTitle(Int n) const		{ return trackTitles.GetNth(n - 1); }
	};
};

#endif
