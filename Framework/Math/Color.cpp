#include "stdafx.h"
#include "Color.h"

Color::Color(const uint & rhs)
{
	const float f = 1.0f / 255.0f;

	r = f * ((rhs >> 16) & 0x000000ff);
	g = f * ((rhs >> 8) & 0x000000ff);
	b = f * ((rhs >> 0) & 0x000000FF);
	a = f * ((rhs >> 24) & 0x000000FF);
}
