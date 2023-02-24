#pragma once

#include "Engine/Math.hpp"

#include <vector>

namespace vi
{
	struct Texture
	{
		i32 width = 0;
		i32 height = 0;
		i32 channels = 0;
		std::vector<unsigned char> pixels;
	};
}