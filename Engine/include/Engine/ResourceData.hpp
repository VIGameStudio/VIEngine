#pragma once

#include "Engine/Resource.hpp"

#include <unordered_set>
#include <unordered_map>

namespace Gaun
{
	struct ShaderData
	{
		std::string text;
	};

	struct TextureData
	{
		i32 width = 0;
		i32 height = 0;
		i32 channels = 0;
		std::vector<unsigned char> pixels;
	};

	struct ModelData
	{
		struct Vertex
		{
			glm::vec4 position = glm::vec4(0, 0, 0, 1);
			glm::vec4 normal = glm::vec4(0, 0, 0, 1);
			glm::vec4 tangent = glm::vec4(0, 0, 0, 1);
			glm::vec4 bitangent = glm::vec4(0, 0, 0, 1);

			glm::vec2 texcoord0 = glm::vec2(0);
			glm::vec2 texcoord1 = glm::vec2(0);
			glm::vec2 texcoord2 = glm::vec2(0);
			glm::vec2 texcoord3 = glm::vec2(0);
			glm::vec4 bones = glm::vec4(-1);
			glm::vec4 weights = glm::vec4(0);
		};

		struct Material
		{
			glm::vec4 diffuse = glm::vec4(1.0f);

			Resource<TextureData> diffuseMap;
			Resource<TextureData> normalMap;
			Resource<TextureData> emmisionMap;

			f32 metallic = 0.1f;
			f32 roughness = 0.9f;
			f32 ao = 0.5f;

			Resource<TextureData> metallicMap;
			Resource<TextureData> roughnessMap;
			Resource<TextureData> aoMap;
		};

		struct Bone
		{
			std::size_t idx = 0;
			glm::mat4 offset = glm::mat4(1);
		};

		struct Armature
		{
			std::unordered_map<std::string, Bone> bones;
		};

		struct Animation
		{
			f32 duration = 0.f;
			f32 speed = 0.f;
			std::unordered_map<std::string, std::vector<glm::mat4>> keyframes;
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