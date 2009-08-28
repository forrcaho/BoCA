 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_ARGSPARSER
#define H_OBJSMOOTH_ARGSPARSER

namespace smooth
{
	class ArgumentsParser;
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI ArgumentsParser
	{
		private:
			Array<String>		 args;
		public:
						 ArgumentsParser(const String &);
						~ArgumentsParser();

			const Array<String>	&GetArguments() const;
	};
};

#endif
