 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_COLOR
#define H_OBJSMOOTH_COLOR

namespace smooth
{
	namespace GUI
	{
		class Color;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 RGBA	= 0;	// Red, Green, Blue, Alpha
		const Short	 HSV	= 1;	// Hue, Saturation, Value
		const Short	 YUV	= 2;	// Value, Blue, Red
		const Short	 CMY	= 3;	// Cyan, Magenta, Yellow (subtractive)
		const Short	 CMYK	= 4;	// Cyan, Magenta, Yellow, Black
		const Short	 GRAY	= 5;	// Black

		class SMOOTHAPI Color
		{
			protected:
				Short		 colorSpace;
				Long		 color;
			public:
						 Color()					{ color = 0; colorSpace = RGBA; }
						 Color(const Color &iColor)			{ color = iColor.color; colorSpace = iColor.colorSpace; }
						 Color(Long iColor, Int iColorSpace = RGBA)	{ color = iColor; colorSpace = iColorSpace; }
						 Color(Int r, Int g, Int b, Int c = RGBA)	{ SetColor(r, g, b, c); }

				Int		 GetRed() const					{ return  color	       & 255; }
				Int		 GetGreen() const				{ return (color >>  8) & 255; }
				Int		 GetBlue() const				{ return (color >> 16) & 255; }
				Int		 GetAlpha() const				{ return (color >> 24) & 255; }

				Void		 SetColor(Long nColor, Int nColorSpace = RGBA)	{ color = nColor; colorSpace = nColorSpace; }
				Void		 SetColor(Int r, Int g, Int b, Int c = RGBA)	{ color = b << 16 | g << 8 | r; colorSpace = c; }

				Color		 ConvertTo(Int) const;
				Color		 Grayscale() const				{ return Color(ConvertTo(GRAY), ConvertTo(GRAY), ConvertTo(GRAY)); }

				Color		 Average(const Color &color2) const		{ return Color( (GetRed()   + color2.GetRed())	 / 2	    |
													       ((GetGreen() + color2.GetGreen()) / 2) <<  8 |
													       ((GetBlue()  + color2.GetBlue())	 / 2) << 16 |
													       ((GetAlpha() + color2.GetAlpha()) / 2) << 24); }

				Color		 Downsample(Int) const;
				Color		 Upsample(Int) const;

				Color &operator	 =(const Long nColor)				{ SetColor(nColor); return *this; }
				operator	 Long() const					{ return color; }
		};
	};
};

#endif
