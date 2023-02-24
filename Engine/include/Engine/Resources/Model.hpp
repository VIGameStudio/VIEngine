#pragma once

#include "Engine/Math.hpp"
#include "Engine/Resource.hpp"
#include "Engine/Resources/Texture.hpp"

#include <unordered_set>
#include <unordered_map>

namespace vi
{
	struct Model
	{
		struct Vertex
		{
			vec4 position = vec4(0, 0, 0, 1);
			vec4 normal = vec4(0, 0, 0, 1);
			vec4 tangent = vec4(0, 0, 0, 1);
			vec4 bitangent = vec4(0, 0, 0, 1);

			vec2 texcoord0 = vec2(0);
			vec2 texcoord1 = vec2(0);
			vec2 texcoord2 = vec2(0);
			vec2 texcoord3 = vec2(0);

			vec4 bones = vec4(-1);
			vec4 weights = vec4(0);
		};

		struct Material
		{
			vec4 diffuse = vec4(1.0f);

			Resource<Texture> diffuseMap;
			Resource<Texture> normalMap;
			Resource<Texture> emmisionMap;

			f32 metallic = 0.1f;
			f32 roughness = 0.9f;
			f32 ao = 0.5f;

			Resource<Texture> metallicMap;
			Resource<Texture> roughnessMap;
			Resource<Texture> aoMap;
		};

		struct Bone
		{
			std::size_t idx = 0;
			mat4 offset = mat4(1);
		};

		struct Armature
		{
			std::unordered_map<std::string, Bone> bones;
		};

		struct Animation
		{
			f32 duration = 0.f;
			f32 speed = 0.f;
			std::unordered_map<std::string, std::vector<mat4>> keyframes;
		};

		struct Mesh
		{
			i32 materialIdx = -1;

			std::vector<Vertex> vertices;
			std::vector<u16> indices;

			Armature armature;
		};

		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		std::vector<Animation> animations;
	};
}