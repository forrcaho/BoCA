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

BoCA_BEGIN_COMPONENT(EncoderMultiEncoderHub)

namespace BoCA
{
	class EncoderMultiEncoderHub : public CS::EncoderComponent
	{
		private:
			ConfigLayer			*configLayer;

			static Config			*configuration;

			static Array<Track>		 tracksToConvert;
			static Array<Track>		 convertedTracks;

			static Track			 playlistTrack;

			Array<IO::OutStream *>		 streams;
			Array<AS::EncoderComponent *>	 encoders;

			Array<Threads::Mutex *>		 mutexes;
			Array<Buffer<UnsignedByte> *>	 buffers;
			Array<Threads::Thread *>	 threads;

			Int64				 trackLength;
			Int64				 totalLength;

			Bool				 finished;
			Bool				 cancelled;

			static String			 GetFileNamePattern(const Track &);
			static String			 GetPlaylistFileName(const Track &);

			Void				 EncodeThread(Int);
		public:
			static const String		&GetComponentSpecs();

							 EncoderMultiEncoderHub();
							~EncoderMultiEncoderHub();

			Bool				 IsThreadSafe() const;

			Bool				 IsLossless() const;

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 WriteData(Buffer<UnsignedByte> &);

			String				 GetOutputFileExtension() const;

			ConfigLayer			*GetConfigurationLayer();
		slots:
			static Void			 OnStartConversion(const Array<Track> &);
			static Void			 OnFinishConversion();
			static Void			 OnCancelConversion();

			Void				 OnFinishTrackConversion(const Track &);
			Void				 OnCancelTrackConversion(const Track &);
	};
};

BoCA_DEFINE_ENCODER_COMPONENT(EncoderMultiEncoderHub)

BoCA_END_COMPONENT(EncoderMultiEncoderHub)
