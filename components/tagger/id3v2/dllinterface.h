 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>

#include "id3/id3.h"

using namespace smooth;
using namespace smooth::System;

extern DynamicLoader	*id3dll;

Bool			 LoadID3DLL();
Void			 FreeID3DLL();

typedef ID3Tag *			(*ID3TAGNEW)			();
typedef void				(*ID3TAGDELETE)			(ID3Tag *);
typedef void				(*ID3TAGSETPADDING)		(ID3Tag *, bool);
typedef void				(*ID3TAGADDFRAME)		(ID3Tag *, const ID3Frame *);
typedef size_t				(*ID3TAGNUMFRAMES)		(ID3Tag *);
typedef ID3Frame *			(*ID3TAGFINDFRAMEWITHID)	(const ID3Tag *, ID3_FrameID);
typedef size_t				(*ID3TAGPARSEBUFFER)		(ID3Tag *tag, const uchar *buffer, size_t bytes);
typedef size_t				(*ID3TAGSIZE)			(const ID3Tag *);
typedef size_t				(*ID3TAGRENDER)			(const ID3Tag *, uchar *, ID3_TagType);
typedef ID3TagIterator *		(*ID3TAGCREATEITERATOR)		(ID3Tag *);
typedef ID3Frame *			(*ID3TAGITERATORGETNEXT)	(ID3TagIterator *);
typedef void				(*ID3TAGITERATORDELETE)		(ID3TagIterator *);
typedef ID3_FrameID			(*ID3FRAMEGETID)		(const ID3Frame *);
typedef ID3Frame *			(*ID3FRAMENEWID)		(ID3_FrameID);
typedef void				(*ID3FRAMEDELETE)		(ID3Frame *);
typedef ID3Field *			(*ID3FRAMEGETFIELD)		(const ID3Frame *, ID3_FieldID);
typedef size_t				(*ID3FIELDSIZE)			(const ID3Field *);
typedef void				(*ID3FIELDSETBINARY)		(ID3Field *, const uchar *, size_t);
typedef void				(*ID3FIELDGETBINARY)		(const ID3Field *, uchar *, size_t);
typedef void				(*ID3FIELDSETINT)		(const ID3Field *, uint32);
typedef uint32				(*ID3FIELDGETINT)		(const ID3Field *);
typedef bool				(*ID3FIELDSETENCODING)		(ID3Field *, ID3_TextEnc);
typedef void				(*ID3FIELDSETASCII)		(ID3Field *, const char *);
typedef size_t				(*ID3FIELDGETASCII)		(const ID3Field *, char *, size_t);
typedef void				(*ID3FIELDSETUNICODE)		(ID3Field *, unicode_t *);
typedef size_t				(*ID3FIELDGETUNICODE)		(const ID3Field *, unicode_t *, size_t);

extern ID3TAGNEW			 ex_ID3Tag_New;
extern ID3TAGDELETE			 ex_ID3Tag_Delete;
extern ID3TAGSETPADDING			 ex_ID3Tag_SetPadding;
extern ID3TAGADDFRAME			 ex_ID3Tag_AddFrame;
extern ID3TAGNUMFRAMES			 ex_ID3Tag_NumFrames;
extern ID3TAGFINDFRAMEWITHID		 ex_ID3Tag_FindFrameWithID;
extern ID3TAGPARSEBUFFER		 ex_ID3Tag_ParseBuffer;
extern ID3TAGSIZE			 ex_ID3Tag_Size;
extern ID3TAGRENDER			 ex_ID3Tag_Render;
extern ID3TAGCREATEITERATOR		 ex_ID3Tag_CreateIterator;
extern ID3TAGITERATORGETNEXT		 ex_ID3TagIterator_GetNext;
extern ID3TAGITERATORDELETE		 ex_ID3TagIterator_Delete;
extern ID3FRAMEGETID			 ex_ID3Frame_GetID;
extern ID3FRAMENEWID			 ex_ID3Frame_NewID;
extern ID3FRAMEDELETE			 ex_ID3Frame_Delete;
extern ID3FRAMEGETFIELD			 ex_ID3Frame_GetField;
extern ID3FIELDSIZE			 ex_ID3Field_Size;
extern ID3FIELDSETBINARY		 ex_ID3Field_SetBINARY;
extern ID3FIELDGETBINARY		 ex_ID3Field_GetBINARY;
extern ID3FIELDSETINT			 ex_ID3Field_SetINT;
extern ID3FIELDGETINT			 ex_ID3Field_GetINT;
extern ID3FIELDSETENCODING		 ex_ID3Field_SetEncoding;
extern ID3FIELDSETASCII			 ex_ID3Field_SetASCII;
extern ID3FIELDGETASCII			 ex_ID3Field_GetASCII;
extern ID3FIELDSETUNICODE		 ex_ID3Field_SetUNICODE;
extern ID3FIELDGETUNICODE		 ex_ID3Field_GetUNICODE;
