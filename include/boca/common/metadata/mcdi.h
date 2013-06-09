 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_MCDI
#define H_BOCA_MCDI

#include <smooth.h>
#include "../../core/definitions.h"

using namespace smooth;

namespace BoCA
{
	enum EntryType
	{
		ENTRY_AUDIO = 0,
		ENTRY_DATA,
		ENTRY_LEADOUT,

		NUM_ENTRYTYPES
	};

	class BOCA_DLL_EXPORT MCDI
	{
		public:
			/* Actual MCDI data:
			 */
			Buffer<UnsignedByte>		 data;
			
			/* Class constructor / destructor:
			 */
							 MCDI(const Buffer<UnsignedByte> &);
							 MCDI(const MCDI &);
							~MCDI();

			MCDI &operator			 =(const MCDI &);

			Int				 GetNumberOfEntries() const;

			Int				 GetNthEntryOffset(Int) const;
			Int				 GetNthEntryType(Int) const;
			Int				 GetNthEntryTrackNumber(Int) const;

			Int				 GetNumberOfAudioTracks() const;
			Int				 GetNumberOfDataTracks() const;

			String				 GetOffsetString() const;
		accessors:
			Bool				 SetData(const Buffer<UnsignedByte> &);
			const Buffer<UnsignedByte>	&GetData() const;
	};
};

#endif