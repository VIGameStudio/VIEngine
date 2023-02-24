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

	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;

	using ivec2 = glm::ivec2;
	using ivec3 = glm::ivec3;
	using ivec4 = glm::ivec4;

	using quat = glm::quat;

	using mat2 = glm::mat2;
	using mat3 = glm::mat3;
	using mat4 = glm::mat4;
}