#pragma once

#define GLM_LANG_STL11_FORCED
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>

#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vi
{
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;

	using i16 = short;
	using i32 = int;
	using i64 = long long;

	using f32 = float;
}