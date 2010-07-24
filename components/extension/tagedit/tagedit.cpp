 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>

#include "tagedit.h"

const String &BoCA::TagEdit::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>Tag Editor</name>			\
	    <version>1.0</version>			\
	    <id>tagedit-ext</id>			\
	    <type>extension</type>			\
	  </component>					\
							\
	";

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
}

Void smooth::DetachDLL()
{
}

BoCA::TagEdit::TagEdit()
{
	mainTabLayer = NIL;

	getMainTabLayer.Connect(&TagEdit::GetMainTabLayer, this);
}

BoCA::TagEdit::~TagEdit()
{
	if (mainTabLayer != NIL) Object::DeleteObject(mainTabLayer);
}

Layer *BoCA::TagEdit::GetMainTabLayer()
{
	if (mainTabLayer == NIL) mainTabLayer = new LayerTags();

	return mainTabLayer;
}
