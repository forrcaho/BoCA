 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OPUSCONFIG
#define H_OPUSCONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureOpus : public ConfigLayer
	{
		private:
			GroupBox	*group_basic;
			Text		*text_mode;
			ComboBox	*combo_mode;
			Text		*text_bandwidth;
			ComboBox	*combo_bandwidth;

			GroupBox	*group_vbr;
			CheckBox	*check_vbr;

			GroupBox	*group_quality;
			Text		*text_bitrate;
			Slider		*slider_bitrate;
			EditBox		*edit_bitrate;
			Text		*text_bitrate_kbps;
			Text		*text_complexity;
			Slider		*slider_complexity;
			Text		*text_complexity_value;

			GroupBox	*group_stream;
			Text		*text_framesize;
			Slider		*slider_framesize;
			Text		*text_framesize_value;

			GroupBox	*group_options;
			CheckBox	*check_dtx;
			Text		*text_packet_loss;
			Slider		*slider_packet_loss;
			Text		*text_packet_loss_value;

			Int		 bitrate;
			Int		 complexity;
			Int		 framesize;
			Int		 packet_loss;

			Bool		 enableVBR;
			Bool		 enableDTX;
		slots:
			Void		 SetMode();

			Void		 SetBitrate();
			Void		 SetBitrateByEditBox();

			Void		 SetComplexity();

			Void		 SetFrameSize();

			Void		 SetPacketLoss();
		public:
					 ConfigureOpus();
					~ConfigureOpus();

			Int		 SaveSettings();
	};
};

#endif
