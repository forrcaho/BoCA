 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_MADCONFIG
#define H_MADCONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureMAD : public ConfigLayer
	{
		private:
			GroupBox	*group_decoding;
			CheckBox	*check_enable24bit;

			Bool		 enable24bit;
		public:
					 ConfigureMAD();
					~ConfigureMAD();

			Int		 SaveSettings();
	};
};

#endif