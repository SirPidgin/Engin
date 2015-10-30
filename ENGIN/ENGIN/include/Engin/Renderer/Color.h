#pragma once

#include <GL\glew.h>

namespace Engin
{
	namespace Renderer
	{
		struct Color
		{
			GLfloat r;
			GLfloat g;
			GLfloat b;
		};

		static const Color clrRed = { 1.0f, 0.0f, 0.0f };
		static const Color clrBlue = { 0.0f, 0.0f, 1.0f };
		static const Color clrGreen = { 0.0f, 1.0f, 0.0, };
		static const Color clrWhite = { 1.0f, 1.0f, 1.0f };
		static const Color clrBlack = { 0.0f, 0.0f, 0.0f };
	}
}
