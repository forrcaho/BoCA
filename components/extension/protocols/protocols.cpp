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
#include <smooth/dll.h>

#include "protocols.h"

const String &BoCA::Protocols::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>Protocol viewer</name>		\
	    <version>1.0</version>			\
	    <id>protocols-ext</id>			\
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

BoCA::Protocols::Protocols()
{
	mainTabLayer = new LayerProtocols();

	getMainTabLayer.Connect(mainTabLayer);
}

BoCA::Protocols::~Protocols()
{
	Object::DeleteObject(mainTabLayer);
}
