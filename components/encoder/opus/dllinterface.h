 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2014 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>

#include <ogg/ogg.h>
#include <opus/opus.h>

using namespace smooth;
using namespace smooth::System;

extern DynamicLoader	*oggdll;
extern DynamicLoader	*opusdll;

Bool			 LoadOggDLL();
Void			 FreeOggDLL();

Bool			 LoadOpusDLL();
Void			 FreeOpusDLL();

/* Ogg API functions.
 */
typedef int			(*OGGSTREAMINIT)	 (ogg_stream_state *, int);
typedef int			(*OGGSTREAMPACKETIN)	 (ogg_stream_state *, ogg_packet *);
typedef int			(*OGGSTREAMFLUSH)	 (ogg_stream_state *, ogg_page *);
typedef int			(*OGGSTREAMPAGEOUT)	 (ogg_stream_state *, ogg_page *);
typedef int			(*OGGPAGEEOS)		 (ogg_page *);
typedef int			(*OGGPAGECHECKSUMSET)	 (ogg_page *);
typedef int			(*OGGSTREAMCLEAR)	 (ogg_stream_state *);

extern OGGSTREAMINIT		 ex_ogg_stream_init;
extern OGGSTREAMPACKETIN	 ex_ogg_stream_packetin;
extern OGGSTREAMFLUSH		 ex_ogg_stream_flush;
extern OGGSTREAMPAGEOUT		 ex_ogg_stream_pageout;
extern OGGPAGEEOS		 ex_ogg_page_eos;
extern OGGPAGECHECKSUMSET	 ex_ogg_page_checksum_set;
extern OGGSTREAMCLEAR		 ex_ogg_stream_clear;

/* Opus API functions.
 */
typedef OpusEncoder *		(*OPUSENCODERCREATE)	 (opus_int32, int, int, int *);
typedef int			(*OPUSENCODE)		 (OpusEncoder *, const opus_int16 *, int, unsigned char *, opus_int32);
typedef int			(*OPUSENCODERCTL)	 (OpusEncoder *, int, ...);
typedef void			(*OPUSENCODERDESTROY)	 (OpusEncoder *);
typedef const char *		(*OPUSGETVERSIONSTRING)	 ();

extern OPUSENCODERCREATE	 ex_opus_encoder_create;
extern OPUSENCODE		 ex_opus_encode;
extern OPUSENCODERCTL		 ex_opus_encoder_ctl;
extern OPUSENCODERDESTROY	 ex_opus_encoder_destroy;
extern OPUSGETVERSIONSTRING	 ex_opus_get_version_string;
