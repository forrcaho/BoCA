 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_CDPARANOIACONFIG
#define H_CDPARANOIACONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureCDParanoia : public ConfigLayer
	{
		private:
			GroupBox		*group_drive;
			ComboBox		*combo_drive;
			CheckBox		*check_speed;
			ComboBox		*combo_speed;
			CheckBox		*check_spinup;
			Slider			*slider_spinup;
			Text			*text_spinup_seconds;
			CheckBox		*check_offset;
			EditBox			*edit_offset;
			Text			*text_offset_samples;

			GroupBox		*group_ripping;
			CheckBox		*check_paranoia;
			ComboBox		*combo_paranoia_mode;

			GroupBox		*group_automatization;
			CheckBox		*check_autoRead;
			CheckBox		*check_autoRip;

			Bool			 setspeed;
			Bool			 spinup;
			Bool			 useoffset;
			Bool			 cdparanoia;

			Bool			 autoRead;
			Bool			 autoRip;

			Array<Bool>		 driveOffsetUsed;
			Array<Int>		 driveOffsets;
			Array<Int>		 driveSpeeds;
			Array<Int>		 driveSpinUpTimes;
		slots:
			Void			 SelectDrive();

			Void			 ToggleSetSpeed();
			Void			 SelectSpeed();

			Void			 ToggleSpinUp();
			Void			 ChangeSpinUpTime();

			Void			 ToggleUseOffset();
			Void			 ChangeOffset();

			Void			 ToggleParanoia();

			Void			 ToggleAutoRead();
		public:
			static const String	 ConfigID;

						 ConfigureCDParanoia();
						~ConfigureCDParanoia();

			Int			 SaveSettings();
	};
};

#endif
