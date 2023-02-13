#pragma once

#include "Engine/Math.hpp"
#include "Engine/Utils.hpp"
#include "Engine/Resource.hpp"
#include "Engine/ResourceData.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace Gaun
{
	using Shader = u32;
	constexpr Shader INVALID_SHADER = 0;

	using Buffer = u32;
	constexpr Buffer INVALID_BUFFER = 0;

	using Texture = u32;
	constexpr Texture INVALID_TEXTURE = 0;

	struct Camera
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	};

	struct DirLight
	{
		glm::vec3 direction = glm::vec3(0.0f);;
		glm::vec3 color = glm::vec3(1.0f);
		f32 intensity = 1.0f;
	};

	struct PointLight
	{
		glm::vec3 position = glm::vec3(0.0f);;
		glm::vec3 color = glm::vec3(1.0f);
		f32 intensity = 1.0f;
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

		Resource<ShaderData> modelShader;
		std::vector<Resource<ModelData>> models;

	private:
		friend class Graphics;
		Settings settings;

		Shader shader;
		Buffer buffer;

		Texture animations;
		Texture textures;
		
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
			glm::mat4 matrix = glm::mat4(1.0f);

			glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

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
		Buffer drawDataBuffer = INVALID_BUFFER;

		struct DrawCommand
		{
			u32 primCount = 0;
			u32 instanceCount = 0;
			u32 firstIndex = 0;
			u32 baseVertex = 0;
			u32 baseInstance = 0;
		};
		std::vector<DrawCommand> commands;
		Buffer commandsBuffer = INVALID_BUFFER;

		struct InstanceData
		{
			glm::mat4 matrix = glm::mat4(1);
			std::string animClip;
			f32 time = 0.f;
		};

		struct DrawBatch
		{
			Resource<ModelData> model;
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

		static void DrawStatic(Pipeline& pipeline, const Resource<ModelData>& model, const glm::mat4& matrix);
		static void DrawSkinned(Pipeline& pipeline, const Resource<ModelData>& model, const glm::mat4& matrix, const std::string& animClip, f32 time);

		static void GenerateDrawCommands(Pipeline& pipeline);
		static void ClearFrame(const Scene& scene);
		static void DrawCommands(const Scene& scene, const Pipeline& pipeline);
		static void ClearDrawCommands(Pipeline& pipeline);

		static void DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color);
		static void DrawDebugShape(const std::vector<glm::mat4>& lines);
		static void DrawDebugCommands(const Scene& scene);
		static void ClearDebugCommands();

	private:
		static void LoadShader(Pipeline& pipeline, const Resource<ShaderData>& resource);
		static void LoadBuffer(Pipeline& pipeline, const std::vector<Resource<ModelData>>& resources);
		static void LoadTexture(Pipeline& pipeline, const std::vector<Resource<TextureData>>& resources);

	private:
		friend class Application;

		static bool Initialize();
		static void Shutdown();

		static void NewFrame();
		static void Render();
	};
}