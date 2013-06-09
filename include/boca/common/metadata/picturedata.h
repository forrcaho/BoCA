 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_PICTUREDATA
#define H_BOCA_PICTUREDATA

#include <smooth.h>
#include "../../core/definitions.h"

using namespace smooth;

namespace BoCA
{
	class BOCA_DLL_EXPORT PictureData
	{
		private:
			static Array<Buffer<UnsignedByte> *>	 dataStore;
			static Array<Short>			 referenceStore;

			Int					 crc;
		public:
								 PictureData();
								 PictureData(const PictureData &);
								~PictureData();

			Int					 Size() const;
			Bool					 Clean();

			Bool					 Set(Void *, Int);

			PictureData &operator			 =(const PictureData &);
			PictureData &operator			 =(const Buffer<UnsignedByte> &);

			operator				 const Buffer<UnsignedByte> &() const;
			operator				 const UnsignedByte *() const;
	};
};

#endif