 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2014 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <windows.h>

namespace CA
{
#include "CoreAudio/AudioFile.h"
#include "CoreAudio/AudioConverter.h"
#include "CoreAudio/AudioFormat.h"
};

extern HINSTANCE	 corefoundationdll;
extern HINSTANCE	 coreaudiodll;

bool			 LoadCoreFoundationDLL();
void			 FreeCoreFoundationDLL();

bool			 LoadCoreAudioDLL();
void			 FreeCoreAudioDLL();

namespace CA
{
	typedef CFStringRef			(*CFSTRINGCREATEWITHCSTRING)		(CFAllocatorRef, const char *, CFStringEncoding);
	typedef CFURLRef			(*CFURLCREATEWITHFILESYSTEMPATH)	(CFAllocatorRef, CFStringRef, CFURLPathStyle, Boolean);
	typedef void				(*CFRELEASE)				(CFTypeRef);

	typedef OSStatus			(*AUDIOFILECREATEWITHURL)		(CFURLRef, AudioFileTypeID, const AudioStreamBasicDescription *, UInt32, AudioFileID *);
	typedef OSStatus			(*AUDIOFILECLOSE)			(AudioFileID);
	typedef OSStatus			(*AUDIOFILESETPROPERTY)			(AudioFileID, AudioFilePropertyID, UInt32, const void *);
	typedef OSStatus			(*AUDIOFILEWRITEPACKETS)		(AudioFileID, Boolean, UInt32, const AudioStreamPacketDescription *, SInt64, UInt32 *, const void *);

	typedef OSStatus			(*AUDIOCONVERTERNEW)			(const AudioStreamBasicDescription *, const AudioStreamBasicDescription *, AudioConverterRef *);
	typedef OSStatus			(*AUDIOCONVERTERDISPOSE)		(AudioConverterRef);
	typedef OSStatus			(*AUDIOCONVERTERGETPROPERTY)		(AudioConverterRef, AudioConverterPropertyID, UInt32 *, void *);
	typedef OSStatus			(*AUDIOCONVERTERGETPROPERTYINFO)	(AudioConverterRef, AudioConverterPropertyID, UInt32 *, Boolean *);
	typedef OSStatus			(*AUDIOCONVERTERSETPROPERTY)		(AudioConverterRef, AudioConverterPropertyID, UInt32, const void *);
	typedef OSStatus			(*AUDIOCONVERTERFILLCOMPLEXBUFFER)	(AudioConverterRef, AudioConverterComplexInputDataProc, void *, UInt32 *, AudioBufferList *, AudioStreamPacketDescription *);

	typedef OSStatus			(*AUDIOFORMATGETPROPERTY)		(AudioFormatPropertyID, UInt32, const void *, UInt32 *, void *);
	typedef OSStatus			(*AUDIOFORMATGETPROPERTYINFO)		(AudioFormatPropertyID, UInt32, const void *, UInt32 *);

	extern CFSTRINGCREATEWITHCSTRING	 CFStringCreateWithCString;
	extern CFURLCREATEWITHFILESYSTEMPATH	 CFURLCreateWithFileSystemPath;
	extern CFRELEASE			 CFRelease;

	extern AUDIOFILECREATEWITHURL		 AudioFileCreateWithURL;
	extern AUDIOFILECLOSE			 AudioFileClose;
	extern AUDIOFILESETPROPERTY		 AudioFileSetProperty;
	extern AUDIOFILEWRITEPACKETS		 AudioFileWritePackets;

	extern AUDIOCONVERTERNEW		 AudioConverterNew;
	extern AUDIOCONVERTERDISPOSE		 AudioConverterDispose;
	extern AUDIOCONVERTERGETPROPERTY	 AudioConverterGetProperty;
	extern AUDIOCONVERTERGETPROPERTYINFO	 AudioConverterGetPropertyInfo;
	extern AUDIOCONVERTERSETPROPERTY	 AudioConverterSetProperty;
	extern AUDIOCONVERTERFILLCOMPLEXBUFFER	 AudioConverterFillComplexBuffer;

	extern AUDIOFORMATGETPROPERTY		 AudioFormatGetProperty;
	extern AUDIOFORMATGETPROPERTYINFO	 AudioFormatGetPropertyInfo;
};