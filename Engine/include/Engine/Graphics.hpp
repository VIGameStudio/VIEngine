#pragma once

#include "Engine/Math.hpp"
#include "Engine/Resource.hpp"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace vi
{
	using ShaderHandle = u32;
	constexpr ShaderHandle INVALID_SHADER = 0;

	using BufferHandle = u32;
	constexpr BufferHandle INVALID_BUFFER = 0;

	using TextureHandle = u32;
	constexpr TextureHandle INVALID_TEXTURE = 0;

	enum struct GraphicsApi { OPENGL, OPENGLES, VULKAN, DX12 };

	enum struct ShaderLang { UNKNOWN, GLSL, HLSL };

	struct Camera
	{
		vec3 position = vec3(0.0f);
		vec3 rotation = vec3(0.0f);
		mat4 view = mat4(1.0f);
		mat4 projection = mat4(1.0f);

		vec4 clearColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	};

	struct DirLight
	{
		vec3 direction = vec3(0.0f);;
		vec3 color = vec3(1.0f);
		f32 intensity = 1.0f;
	};

	struct PointLight
	{
		vec3 position = vec3(0.0f);;
		vec3 color = vec3(1.0f);
		f32 intensity = 1.0f;
	};

	struct Shader
	{
		ShaderLang lang = ShaderLang::UNKNOWN;
		std::string source;
	};

	struct Texture
	{
		i32 width = 0;
		i32 height = 0;
		i32 channels = 0;
		std::vector<unsigned char> pixels;
	};

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

	// TODO: Move models batching here instead of pipeline
	/*struct Batch
	{
		Resource<ShaderData> modelShader;
		std::vector<Resource<ModelData>> models;

	private:
		friend class Graphics;
		Shader shaders;
		Buffer buffer;
		Texture texture;
	};*/

	struct Scene
	{
		Camera camera;
		DirLight dirLight;
		std::vector<PointLight> pointLights;
	};

	struct Pipeline
	{
		struct Settings
		{
			u32 textureSize = 1024;
		};

		Resource<Shader> modelShader;
		std::vector<Resource<Model>> models;

	private:
		friend class Graphics;
		Settings settings;

		ShaderHandle shader;
		BufferHandle buffer;

		TextureHandle animations;
		TextureHandle textures;
		
		struct MeshInfo
		{
			u32 primCount = 0;
			u32 firstIndex = 0;
			u32 baseVertex = 0;
		};
		std::unordered_map<std::size_t, MeshInfo> meshInfoMap;

		struct TextureInfo
		{
			u32 index = 0;
		};
		std::unordered_map<std::size_t, TextureInfo> textureInfoMap;

		struct DrawData
		{
			mat4 matrix = mat4(1.0f);

			vec4 diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);

			f32 diffuseMap = -1;
			f32 normalMap = -1;
			f32 emmisionMap = -1;
			f32 animationIdx = -1;
			
			f32 metallic = 0.5f;
			f32 roughness = 0.5f;
			f32 ao = 0.5f;
			f32 animationFrames = 0;

			f32 metallicMap = -1;
			f32 roughnessMap = -1;
			f32 aoMap = -1;
			f32 animationTime = 0;
		};

		std::vector<DrawData> drawData;
		BufferHandle drawDataBuffer = INVALID_BUFFER;

		struct DrawCommand
		{
			u32 primCount = 0;
			u32 instanceCount = 0;
			u32 firstIndex = 0;
			u32 baseVertex = 0;
			u32 baseInstance = 0;
		};
		std::vector<DrawCommand> commands;
		BufferHandle commandsBuffer = INVALID_BUFFER;

		struct InstanceData
		{
			mat4 matrix = mat4(1);
			std::string animClip;
			f32 time = 0.f;
		};

		struct DrawBatch
		{
			Resource<Model> model;
			DrawData drawData;
			std::vector<InstanceData> instances;
		};
		std::unordered_map<std::size_t, DrawBatch> drawBatchMap;
	};

	class Graphics
	{
	public:
		static void LoadPipeline(Pipeline& pipeline, const Pipeline::Settings& settings);
		static void UnloadPipeline(Pipeline& pipeline);

		static void DrawStatic(Pipeline& pipeline, const Resource<Model>& model, const mat4& matrix);
		static void DrawSkinned(Pipeline& pipeline, const Resource<Model>& model, const mat4& matrix, const std::string& animClip, f32 time);

		static void GenerateDrawCommands(Pipeline& pipeline);
		static void ClearFrame(const Scene& scene);
		static void DrawCommands(const Scene& scene, const Pipeline& pipeline);
		static void ClearDrawCommands(Pipeline& pipeline);

		static void DrawDebugLine(const vec3& p1, const vec3& p2, const vec4& color);
		static void DrawDebugShape(const std::vector<mat4>& lines);
		static void DrawDebugCommands(const Scene& scene);
		static void ClearDebugCommands();

	private:
		static void LoadShader(Pipeline& pipeline, const Resource<Shader>& resource);
		static void LoadBuffer(Pipeline& pipeline, const std::vector<Resource<Model>>& resources);
		static void LoadTexture(Pipeline& pipeline, const std::vector<Resource<Texture>>& resources);

	private:
		friend class Application;

		static bool Initialize(const void* pLoadProc);
		static void Shutdown();

		static void Render();
	};
}