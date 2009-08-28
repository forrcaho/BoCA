 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_PROGRESSBAR
#define H_OBJSMOOTH_PROGRESSBAR

namespace smooth
{
	namespace GUI
	{
		class Progressbar;
	};
};

#include "../widget.h"
#include "../../../graphics/bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int PB_NOTEXT	= 0;
		const Int PB_VALUE	= 1;
		const Int PB_PERCENT	= 2;

		class SMOOTHAPI Progressbar : public Widget
		{
			private:
				Bitmap			 gradient;

				Void			 CreateGradient(const Size &);
			protected:
				Int			 value;

				Int			 startValue;
				Int			 endValue;
			public:
				static const Int	 classID;

							 Progressbar(const Point &, const Size &, Int = OR_HORZ, Int = PB_PERCENT, Int = 0, Int = 100, Int = 0);
				virtual			~Progressbar();

				virtual Int		 Paint(Int);
			accessors:
				Int			 SetMetrics(const Point &, const Size &);

				Int			 SetValue(Int);
				Int			 GetValue();
		};
	};
};

#endif
