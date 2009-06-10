///////////////////////////////////////////////////////////////////////////
//
// ASFErr.h - definition of ASF HRESULT codes
//
//=========================================================================
//
//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation.  All Rights Reserved.
//
//=========================================================================
//
// This file is generated by the MC tool from ASFErr.mc
//

#ifndef _ASFERR_H
#define _ASFERR_H


#define STATUS_SEVERITY(hr)  (((hr) >> 30) & 0x3)

#ifdef RC_INVOKED
#define _ASF_HRESULT_TYPEDEF_(_sc) _sc
#else // RC_INVOKED
#define _ASF_HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#endif // RC_INVOKED


///////////////////////////////////////////////////////////////////////////
//
// Advanced Streaming Format (ASF) Errors (2000 - 2999)
//
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_NS                      0xD


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: ASF_E_BUFFEROVERRUN
//
// MessageText:
//
// An attempt was made to seek or position past the end of a buffer.%0
//
#define ASF_E_BUFFEROVERRUN              _ASF_HRESULT_TYPEDEF_(0xC00D07D0L)

//
// MessageId: ASF_E_BUFFERTOOSMALL
//
// MessageText:
//
// The supplied input or output buffer was too small.%0
//
#define ASF_E_BUFFERTOOSMALL             _ASF_HRESULT_TYPEDEF_(0xC00D07D1L)

//
// MessageId: ASF_E_BADLANGUAGEID
//
// MessageText:
//
// The language ID was not found.%0
//
#define ASF_E_BADLANGUAGEID              _ASF_HRESULT_TYPEDEF_(0xC00D07D2L)

//
// MessageId: ASF_E_NOPAYLOADLENGTH
//
// MessageText:
//
// The multiple payload packet is missing the payload length.%0
//
#define ASF_E_NOPAYLOADLENGTH            _ASF_HRESULT_TYPEDEF_(0xC00D07DBL)

//
// MessageId: ASF_E_TOOMANYPAYLOADS
//
// MessageText:
//
// The packet contains too many payloads.%0
//
#define ASF_E_TOOMANYPAYLOADS            _ASF_HRESULT_TYPEDEF_(0xC00D07DCL)

//
// MessageId: ASF_E_PACKETCONTENTTOOLARGE
//
// MessageText:
//
//  ASF_E_PACKETCONTENTTOOLARGE
//
#define ASF_E_PACKETCONTENTTOOLARGE      _ASF_HRESULT_TYPEDEF_(0xC00D07DEL)

//
// MessageId: ASF_E_UNKNOWNPACKETSIZE
//
// MessageText:
//
// Expecting a fixed packet size but min. and max. are not equal.%0
//
#define ASF_E_UNKNOWNPACKETSIZE          _ASF_HRESULT_TYPEDEF_(0xC00D07E0L)

//
// MessageId: ASF_E_INVALIDHEADER
//
// MessageText:
//
//  ASF_E_INVALIDHEADER
//
#define ASF_E_INVALIDHEADER              _ASF_HRESULT_TYPEDEF_(0xC00D07E2L)

//
// MessageId: ASF_E_NOCLOCKOBJECT
//
// MessageText:
//
// The object does not have a valid clock object.%0
//
#define ASF_E_NOCLOCKOBJECT              _ASF_HRESULT_TYPEDEF_(0xC00D07E6L)

//
// MessageId: ASF_E_UNKNOWNCLOCKTYPE
//
// MessageText:
//
//  ASF_E_UNKNOWNCLOCKTYPE
//
#define ASF_E_UNKNOWNCLOCKTYPE           _ASF_HRESULT_TYPEDEF_(0xC00D07EBL)

//
// MessageId: ASF_E_OPAQUEPACKET
//
// MessageText:
//
// An attempt was made to restore or access an opaque packet.%0
//
#define ASF_E_OPAQUEPACKET               _ASF_HRESULT_TYPEDEF_(0xC00D07EDL)

//
// MessageId: ASF_E_WRONGVERSION
//
// MessageText:
//
//  ASF_E_WRONGVERSION
//
#define ASF_E_WRONGVERSION               _ASF_HRESULT_TYPEDEF_(0xC00D07EEL)

//
// MessageId: ASF_E_OVERFLOW
//
// MessageText:
//
// An attempt was made to store a value which was larger than then destination's maximum value.%0
//
#define ASF_E_OVERFLOW                   _ASF_HRESULT_TYPEDEF_(0xC00D07EFL)

//
// MessageId: ASF_E_NOTFOUND
//
// MessageText:
//
// The object was not found.%0
//
#define ASF_E_NOTFOUND                   _ASF_HRESULT_TYPEDEF_(0xC00D07F0L)

//
// Someone else is using MessageIds 2033 & 2034, so we skip them
// 
// 2033 = NS_E_NOTHING_TO_DO
// 2034 = NS_E_NO_MULTICAST

//
// MessageId: ASF_E_OBJECTTOOBIG
//
// MessageText:
//
// The object is too large to be processed in the requested manner.%0
//
#define ASF_E_OBJECTTOOBIG               _ASF_HRESULT_TYPEDEF_(0xC00D07F3L)

//
// MessageId: ASF_E_UNEXPECTEDVALUE
//
// MessageText:
//
// A value was not set as expected.%0
//
#define ASF_E_UNEXPECTEDVALUE            _ASF_HRESULT_TYPEDEF_(0xC00D07F4L)

//
// MessageId: ASF_E_INVALIDSTATE
//
// MessageText:
//
// The request is not valid in the object's current state.%0
//
#define ASF_E_INVALIDSTATE               _ASF_HRESULT_TYPEDEF_(0xC00D07F5L)

//
// MessageId: ASF_E_NOLIBRARY
//
// MessageText:
//
// This object does not have a valid library pointer; it was not properly created or it has been Shutdown().%0
//
#define ASF_E_NOLIBRARY                  _ASF_HRESULT_TYPEDEF_(0xC00D07F6L)

//
// MessageId: ASF_E_ALREADYINITIALIZED
//
// MessageText:
//
// This object has already been initialized; the setting cannot be changed.%0
//
#define ASF_E_ALREADYINITIALIZED         _ASF_HRESULT_TYPEDEF_(0xC00D07F7L)

//
// MessageId: ASF_E_INVALIDINIT
//
// MessageText:
//
// This object has not been initialized properly; that operation cannot be performed.%0
//
#define ASF_E_INVALIDINIT                _ASF_HRESULT_TYPEDEF_(0xC00D07F8L)

//
// MessageId: ASF_E_NOHEADEROBJECT
//
// MessageText:
//
// The ASF Header object could not be found.%0
//
#define ASF_E_NOHEADEROBJECT             _ASF_HRESULT_TYPEDEF_(0xC00D07F9L)

//
// MessageId: ASF_E_NODATAOBJECT
//
// MessageText:
//
// The ASF Data object could not be found.%0
//
#define ASF_E_NODATAOBJECT               _ASF_HRESULT_TYPEDEF_(0xC00D07FAL)

//
// MessageId: ASF_E_NOINDEXOBJECT
//
// MessageText:
//
// The ASF Index object could not be found.%0
//
#define ASF_E_NOINDEXOBJECT              _ASF_HRESULT_TYPEDEF_(0xC00D07FBL)

//
// MessageId: ASF_E_NOSTREAMPROPS
//
// MessageText:
//
// A Stream Properties object with the correct stream number could not be found.%0
//
#define ASF_E_NOSTREAMPROPS              _ASF_HRESULT_TYPEDEF_(0xC00D07FCL)

//
// MessageId: ASF_E_NOFILEPROPS
//
// MessageText:
//
// The File Properties object could not be found.%0
//
#define ASF_E_NOFILEPROPS                _ASF_HRESULT_TYPEDEF_(0xC00D07FDL)

//
// MessageId: ASF_E_NOLANGUAGELIST
//
// MessageText:
//
// The Language List object could not be found.%0
//
#define ASF_E_NOLANGUAGELIST             _ASF_HRESULT_TYPEDEF_(0xC00D07FEL)

//
// MessageId: ASF_E_NOINDEXPARAMETERS
//
// MessageText:
//
// The Index Parameters object could not be found.%0
//
#define ASF_E_NOINDEXPARAMETERS          _ASF_HRESULT_TYPEDEF_(0xC00D07FFL)

//
// MessageId: ASF_E_UNSUPPORTEDERRORCONCEALMENT
//
// MessageText:
//
// The requested error concealment strategy is not supported by this component.%0
//
#define ASF_E_UNSUPPORTEDERRORCONCEALMENT _ASF_HRESULT_TYPEDEF_(0xC00D0800L)

//
// MessageId: ASF_E_INVALIDFLAGS
//
// MessageText:
//
// The flags for this object or set of objects are not properly set.%0
//
#define ASF_E_INVALIDFLAGS               _ASF_HRESULT_TYPEDEF_(0xC00D0801L)

//
// MessageId: ASF_E_BADDATADESCRIPTOR
//
// MessageText:
//
// One or more data descriptors is not properly set.%0
//
#define ASF_E_BADDATADESCRIPTOR          _ASF_HRESULT_TYPEDEF_(0xC00D0802L)

//
// MessageId: ASF_E_BADINDEXINTERVAL
//
// MessageText:
//
// The index has an invalid time interval (probably zero).%0
//
#define ASF_E_BADINDEXINTERVAL           _ASF_HRESULT_TYPEDEF_(0xC00D0803L)

//
// MessageId: ASF_E_INVALIDTIME
//
// MessageText:
//
// The given time value is not valid.%0
//
#define ASF_E_INVALIDTIME                _ASF_HRESULT_TYPEDEF_(0xC00D0804L)

//
// MessageId: ASF_E_INVALIDINDEX
//
// MessageText:
//
// The given index value is not valid.%0
//
#define ASF_E_INVALIDINDEX               _ASF_HRESULT_TYPEDEF_(0xC00D0805L)

//
// MessageId: ASF_E_STREAMNUMBERINUSE
//
// MessageText:
//
// The specified stream number is already in use.%0
//
#define ASF_E_STREAMNUMBERINUSE          _ASF_HRESULT_TYPEDEF_(0xC00D0806L)

//
// MessageId: ASF_E_BADMEDIATYPE
//
// MessageText:
//
// The specified media type does not work with this component.%0
//
#define ASF_E_BADMEDIATYPE               _ASF_HRESULT_TYPEDEF_(0xC00D0807L)

//
// MessageId: ASF_E_WRITEFAILED
//
// MessageText:
//
// The object could not be written as specified.%0
//
#define ASF_E_WRITEFAILED                _ASF_HRESULT_TYPEDEF_(0xC00D0808L)

//
// MessageId: ASF_E_NOTENOUGHDESCRIPTORS
//
// MessageText:
//
// The given data unit requires a larger number of descriptors to be fully parsed.%0
//
#define ASF_E_NOTENOUGHDESCRIPTORS       _ASF_HRESULT_TYPEDEF_(0xC00D0809L)

//
// MessageId: ASF_E_INDEXBLOCKUNLOADED
//
// MessageText:
//
// The index entries for the specified index block have been unloaded from memory and are not available.%0
//
#define ASF_E_INDEXBLOCKUNLOADED         _ASF_HRESULT_TYPEDEF_(0xC00D080AL)

//
// MessageId: ASF_E_NOTENOUGHBANDWIDTH
//
// MessageText:
//
// The specified bandwidth is not large enough.%0
//
#define ASF_E_NOTENOUGHBANDWIDTH         _ASF_HRESULT_TYPEDEF_(0xC00D080BL)

//
// MessageId: ASF_E_EXCEEDEDMAXIMUMOBJECTSIZE
//
// MessageText:
//
// The object has exceeded its maximum size.%0
//
#define ASF_E_EXCEEDEDMAXIMUMOBJECTSIZE  _ASF_HRESULT_TYPEDEF_(0xC00D080CL)

//
// MessageId: ASF_E_BADDATAUNIT
//
// MessageText:
//
// The given data unit is corrupted, badly formatted, or otherwise not valid.%0
//
#define ASF_E_BADDATAUNIT                _ASF_HRESULT_TYPEDEF_(0xC00D080DL)

//
// MessageId: ASF_E_HEADERSIZE
//
// MessageText:
//
// The ASF header has exceeded the specified maximum size.%0
//
#define ASF_E_HEADERSIZE                 _ASF_HRESULT_TYPEDEF_(0xC00D080EL)


///////////////////////////////////////////////////////////////////////////
//
// Advanced Streaming Format (ASF) Success Codes (2000 - 2999)
//

//
// MessageId: ASF_S_OPAQUEPACKET
//
// MessageText:
//
//  ASF_S_OPAQUEPACKET
//
#define ASF_S_OPAQUEPACKET               _ASF_HRESULT_TYPEDEF_(0x000D07F0L)


///////////////////////////////////////////////////////////////////////////
//
// Advanced Streaming Format (ASF) Warnings (2000 - 2999)
//


#endif // _ASFERR_H

