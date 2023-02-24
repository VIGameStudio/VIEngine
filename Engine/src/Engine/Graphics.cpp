#include "Engine/Graphics.hpp"

#include "Engine/Assert.hpp"
#include "Engine/Log.hpp"
#include "Engine/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <backends/imgui_impl_glfw.h>
//#include <backends/imgui_impl_opengl3.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#if defined WIN32
#define GL_BACKEND
#elif defined __arm__
#define GLES_BACKEND
#endif

#if defined GL_BACKEND
#define GLSL_VERSION "#version 460 core\n"
#elif defined GLES_BACKEND
#define GLSL_VERSION "#version 310 es\n"
#endif

#define GLSL_HEADER "\n"

#define GLSL_VERT_SHADER "#define VERT_SHADER\n"
#define GLSL_FRAG_SHADER "#define FRAG_SHADER\n"

namespace vi
{
    static GLuint Load_GlShader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
    {
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertSrc_cstr = vertSrc.c_str();
        glShaderSource(vertexShader, 1, &vertSrc_cstr, NULL);
        glCompileShader(vertexShader);

        i32 success;
        char infoLog[1024];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
            Log::Error("SHADER_COMPILATION_ERROR of type: {}", infoLog);
        }

        // Compile fragment shader
        GLuint fragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
        const char* fragSrc_cstr = fragSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragSrc_cstr, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
            Log::Error("SHADER_COMPILATION_ERROR of type: {}", infoLog);
        }

        // Link vertex and fragment shader together
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Delete shaders objects
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void Graphics::LoadShader(Pipeline& pipeline, const Resource<Shader>& resource)
    {
        ASSERT(resource.HasData() && "Shader resource not loaded!");

        const std::string src = resource.GetData().text;
        const std::string vertSrc = fmt::format("{}{}{}{}", GLSL_VERSION, GLSL_HEADER, GLSL_VERT_SHADER, src);
        const std::string fragSrc = fmt::format("{}{}{}{}", GLSL_VERSION, GLSL_HEADER, GLSL_FRAG_SHADER, src);

        pipeline.shader = Load_GlShader(vertSrc, fragSrc, "");
    }

    void Graphics::LoadBuffer(Pipeline& pipeline, const std::vector<Resource<Model>>& resources)
    {
        // Allocate memory
        std::size_t vertexCount = 0;
        std::size_t indexCount = 0;

        for (const auto& res : resources)
        {
            ASSERT(res.HasData() && "Mesh resource not loaded!");
            const auto& modelData = res.GetData();

            for (const auto& mesh : modelData.meshes)
            {
                vertexCount += mesh.vertices.size();
                indexCount += mesh.indices.size();
            }
        }

        std::vector<Model::Vertex> vertices(vertexCount);
        std::vector<u16> indices(indexCount);

        // Copy mesh data into buffers
        std::size_t vertexIdx = 0;
        std::size_t indexIdx = 0;
        for (const auto& res : resources)
        {
            Pipeline::DrawBatch drawBatch;
            drawBatch.model = res;
            pipeline.drawBatchMap.insert(std::make_pair(res.GetHash(), drawBatch));

            const auto& modelData = res.GetData();

            std::size_t idx = 0;
            for (const auto& mesh : modelData.meshes)
            {
                Pipeline::MeshInfo meshInfo;
                meshInfo.primCount = (u32)mesh.indices.size();
                meshInfo.firstIndex = (u32)indexIdx;
                meshInfo.baseVertex = (u32)vertexIdx;

                std::size_t hash = res.GetHash() ^ idx++;
                pipeline.meshInfoMap.insert(std::make_pair(hash, meshInfo));
                
                memcpy(&vertices[vertexIdx], mesh.vertices.data(), mesh.vertices.size() * sizeof(Model::Vertex));
                memcpy(&indices[indexIdx], mesh.indices.data(), mesh.indices.size() * sizeof(u16));

                vertexIdx += mesh.vertices.size();
                indexIdx += mesh.indices.size();
            }
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create a vertex buffer object
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Model::Vertex), vertices.data(), GL_STATIC_DRAW);

        // Specify vertex attributes for the shader
        for (i32 i = 0; i < 8; i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (GLvoid*)(i * sizeof(glm::vec4)));
        }

        // Create an element buffer and populate it
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u16), indices.data(), GL_STATIC_DRAW);

        // Generate per instance buffer
        glGenBuffers(1, &pipeline.drawDataBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, pipeline.drawDataBuffer);

        for (u32 i = 0; i < 8; i++)
        {
            glEnableVertexAttribArray(8 + i);
            glVertexAttribPointer(8 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Pipeline::DrawData), (GLvoid*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(8 + i, 1);
        }

        // Generate indirect command buffer
        glGenBuffers(1, &pipeline.commandsBuffer);

        glBindVertexArray(0);

        pipeline.buffer = vao;
    }

    void Graphics::LoadTexture(Pipeline& pipeline, const std::vector<Resource<Texture>>& resources)
    {
        // Generate an array texture
        GLuint tex;
        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

        // Create storage for the texture
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, pipeline.settings.textureSize, pipeline.settings.textureSize, (GLsizei)resources.size());
        //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_SRGB8_ALPHA8, pipeline.settings.textureSize, pipeline.settings.textureSize, (GLsizei)resources.size());

        auto pData = new unsigned char[(long long)4 * pipeline.settings.textureSize * pipeline.settings.textureSize];
        for (u32 i = 0; i < resources.size(); ++i)
        {
            const auto& res = resources[i];
            
//#define DEBUG_GRID_TEXTURE
#ifdef DEBUG_GRID_TEXTURE
            // Texture grid for debugging
            glm::uvec3 color = glm::linearRand(glm::uvec3(0), glm::uvec3(255));
            for (GLsizei y = 0; y < pipeline.settings.textureSize; ++y)
                for (GLsizei x = 0; x < pipeline.settings.textureSize; ++x)
                    for (GLsizei k = 0; k < 4; ++k)
                        pData[y * pipeline.settings.textureSize * 4 + x * 4 + k] = k == 3 ? 255 : (((x / 16) + (y / 16)) % 2) == 0 ? color[k] : 0;
#else
            ASSERT(res.HasData() && "Texture resource not loaded!");
            const auto& data = res.GetData();

            ASSERT(data.channels == 4 && "Only RGBA textures can be loaded!");
            stbir_resize_uint8(data.pixels.data(), data.width, data.height, 0, pData, pipeline.settings.textureSize, pipeline.settings.textureSize, 0, 4);
#endif

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, pipeline.settings.textureSize, pipeline.settings.textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, pData);

            Pipeline::TextureInfo textureInfo;
            textureInfo.index = i;
            pipeline.textureInfoMap.insert(std::make_pair(res.GetHash(), textureInfo));
        }
        delete[] pData;

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        pipeline.textures = tex;
    }

    void Graphics::LoadPipeline(Pipeline& pipeline, const Pipeline::Settings& settings)
    {
        pipeline.settings = settings;

        LoadShader(pipeline, pipeline.modelShader);
        LoadBuffer(pipeline, pipeline.models);

        std::vector<Resource<Texture>> textures;
        for (const auto& res : pipeline.models)
        {
            auto& modelData = res.GetData();
            for (const auto& material : modelData.materials)
            {
                // Diffuse maps
                if (material.diffuseMap.HasData())
                    textures.emplace_back(material.diffuseMap);

                if (material.normalMap.HasData())
                    textures.emplace_back(material.normalMap);

                if (material.emmisionMap.HasData())
                    textures.emplace_back(material.emmisionMap);

                // Pbr maps
                if (material.metallicMap.HasData())
                    textures.emplace_back(material.metallicMap);

                if (material.roughnessMap.HasData())
                    textures.emplace_back(material.roughnessMap);

                if (material.aoMap.HasData())
                    textures.emplace_back(material.aoMap);
            }
        }

        LoadTexture(pipeline, textures);

        // Link resources
        for (auto& it : pipeline.drawBatchMap)
        {
            auto& drawBatch = it.second;
            const auto& modelData = drawBatch.model.GetData();

            for (size_t idx = 0; idx < modelData.meshes.size(); idx++)
            {
                const auto& mesh = modelData.meshes[idx];
                if (mesh.materialIdx >= 0)
                {
                    ASSERT(modelData.materials.size() > mesh.materialIdx && "Model does not contain material index!");
                    const auto& material = modelData.materials[mesh.materialIdx];

                    drawBatch.drawData.diffuse = material.diffuse;
                    drawBatch.drawData.metallic = material.metallic;
                    drawBatch.drawData.roughness = material.roughness;
                    drawBatch.drawData.ao = material.ao;

                    // Diffuse maps
                    if (material.diffuseMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.diffuseMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.diffuseMap = (f32)it2->second.index;
                    }

                    if (material.normalMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.normalMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.normalMap = (f32)it2->second.index;
                    }

                    if (material.diffuseMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.diffuseMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.emmisionMap = (f32)it2->second.index;
                    }

                    // Pbr maps
                    if (material.metallicMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.metallicMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.metallicMap = (f32)it2->second.index;
                    }

                    if (material.roughnessMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.roughnessMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.roughnessMap = (f32)it2->second.index;
                    }

                    if (material.aoMap.HasData())
                    {
                        auto it2 = pipeline.textureInfoMap.find(material.aoMap.GetHash());
                        ASSERT(it2 != pipeline.textureInfoMap.end() && "No texture info for draw call!");
                        drawBatch.drawData.aoMap = (f32)it2->second.index;
                    }
                }
            }
        }
    }

    void Graphics::UnloadPipeline(Pipeline& pipeline)
    {
        ClearDrawCommands(pipeline);
        pipeline.drawBatchMap.clear();
    }

    void Graphics::DrawStatic(Pipeline& pipeline, const Resource<Model>& model, const glm::mat4& matrix)
    {
        auto it = pipeline.drawBatchMap.find(model.GetHash());
        ASSERT(it != pipeline.drawBatchMap.end() && "There are no caches for model!");
        auto& drawBatch = it->second;

        Pipeline::InstanceData instance;
        instance.matrix = matrix;
        drawBatch.instances.emplace_back(instance);
    }

    void Graphics::DrawSkinned(Pipeline& pipeline, const Resource<Model>& model, const glm::mat4& matrix, const std::string& animClip, f32 time)
    {
        auto it = pipeline.drawBatchMap.find(model.GetHash());
        ASSERT(it != pipeline.drawBatchMap.end() && "There are no caches for model!");
        auto& drawBatch = it->second;

        Pipeline::InstanceData instance;
        instance.matrix = matrix;
        instance.animClip = animClip;
        instance.time = time;
        drawBatch.instances.emplace_back(instance);
    }

    void Graphics::GenerateDrawCommands(Pipeline& pipeline)
    {
        u32 baseInstance = 0;
        for (const auto& it : pipeline.drawBatchMap)
        {
            auto& drawBatch = it.second;
            auto& modelData = drawBatch.model.GetData();

            for (size_t idx = 0; idx < modelData.meshes.size(); idx++)
            {
                Pipeline::DrawData drawData = drawBatch.drawData;
                for (const auto& instance : drawBatch.instances)
                {
                    drawData.matrix = instance.matrix;
                    pipeline.drawData.emplace_back(drawData);
                }

                std::size_t hash = drawBatch.model.GetHash() ^ idx;
                auto it3 = pipeline.meshInfoMap.find(hash);
                ASSERT(it3 != pipeline.meshInfoMap.end() && "No mesh info for draw call!");
                const auto& meshInfo = it3->second;

                Pipeline::DrawCommand cmd;
                cmd.primCount = meshInfo.primCount;
                cmd.firstIndex = meshInfo.firstIndex;
                cmd.baseVertex = meshInfo.baseVertex;
                cmd.instanceCount = (u32)drawBatch.instances.size();
                cmd.baseInstance = baseInstance;
                baseInstance += cmd.instanceCount;

                pipeline.commands.emplace_back(cmd);
            }
        }

        //glBindVertexArray(pipeline.buffer);
        
        // Update model matrices and material properties
        //if (pipeline.drawDataBuffer == INVALID_BUFFER)
        //    glGenBuffers(1, &pipeline.drawDataBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, pipeline.drawDataBuffer);
        glBufferData(GL_ARRAY_BUFFER, pipeline.drawData.size() * sizeof(Pipeline::DrawData), pipeline.drawData.data(), GL_DYNAMIC_DRAW);

        /*for (u32 i = 0; i < 8; i++)
        {
            glEnableVertexAttribArray(8 + i);
            glVertexAttribPointer(8 + i, 4, GL_FLOAT, GL_FALSE, sizeof(Pipeline::DrawData), (GLvoid*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(8 + i, 1);
        }*/

        // Generate draw commands
        //if (pipeline.commandsBuffer == INVALID_BUFFER)
        //    glGenBuffers(1, &pipeline.commandsBuffer);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, pipeline.commandsBuffer);
        glBufferData(GL_DRAW_INDIRECT_BUFFER, pipeline.commands.size() * sizeof(Pipeline::DrawCommand), pipeline.commands.data(), GL_DYNAMIC_DRAW);

        //glBindBuffer(GL_ARRAY_BUFFER, pipeline.commandsBuffer);
        //glEnableVertexAttribArray(12);
        //glVertexAttribIPointer(12, 1, GL_UNSIGNED_INT, sizeof(Pipeline::DrawCommand), (void*)(4 * sizeof(u32)));
        //glVertexAttribDivisor(12, 1);

        //glBindVertexArray(0);
    }

    void Graphics::ClearFrame(const Scene& scene)
    {
        i32 width, height;
        Window::GetSize(&width, &height);

        glViewport(0, 0, width, height);
        const glm::vec4& c = scene.camera.clearColor;
        glClearColor(c.r, c.g, c.b, c.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Graphics::DrawCommands(const Scene& scene, const Pipeline& pipeline)
    {
        glCullFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH);
        glEnable(GL_DEPTH_TEST);

        glBindVertexArray(pipeline.buffer);
        glUseProgram(pipeline.shader);

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(scene.camera.view));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(scene.camera.projection));

        glUniform3fv(2, 1, glm::value_ptr(scene.camera.position));
        glUniform3fv(3, 1, glm::value_ptr(scene.dirLight.direction));
        glUniform3fv(4, 1, glm::value_ptr(scene.dirLight.color));
        glUniform1f(5, scene.dirLight.intensity);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D_ARRAY, pipeline.animations);
        //glUniform1i(0, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D_ARRAY, pipeline.textures);
        glUniform1i(1, 0);

#if defined GL_BACKEND
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, (void*)0, (GLsizei)pipeline.commands.size(), 0);

#elif defined GLES_BACKEND
        for (u32 i = 0; i < pipeline.commands.size(); i++)
            glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, (void*)(i * sizeof(Pipeline::DrawCommand)));
#endif

        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Graphics::ClearDrawCommands(Pipeline& pipeline)
    {
        for (auto& it : pipeline.drawBatchMap)
            it.second.instances.clear();

        pipeline.drawData.clear();
        pipeline.commands.clear();
    }

    static ShaderHandle s_debugShader = INVALID_SHADER;
    static BufferHandle s_debugBuffer = INVALID_BUFFER;
    static BufferHandle s_debugBufferObj = INVALID_BUFFER;
    static std::vector<glm::mat4> s_debugLines;

    static void Init_DebugDraw()
    {
        static const char* src =
            "#ifdef VERT_SHADER\n"
            "layout (location = 0) in vec4 iPosition;\n"
            "layout (location = 1) in vec4 iColor;\n"
            "layout (location = 0) out vec4 ioColor;\n"
            "layout (location = 0) uniform mat4 uVP;\n"
            "void main() { ioColor = iColor; gl_Position = uVP * vec4(iPosition.xyz, 1.0); }\n"
            "#endif\n"
            "#ifdef FRAG_SHADER\n"
            "precision mediump float;\n"
            "layout (location = 0) in vec4 ioColor;\n"
            "layout (location = 0) out vec4 oFragColor;\n"
            "void main() { oFragColor = ioColor; }\n"
            "#endif\n";

        const std::string vertSrc = fmt::format("{}{}{}{}", GLSL_VERSION, GLSL_HEADER, GLSL_VERT_SHADER, src);
        const std::string fragSrc = fmt::format("{}{}{}{}", GLSL_VERSION, GLSL_HEADER, GLSL_FRAG_SHADER, src);
        s_debugShader = Load_GlShader(vertSrc, fragSrc, "");

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        for (i32 i = 0; i < 2; i++)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (GLvoid*)(i * sizeof(glm::vec4)));
        }

        glBindVertexArray(0);
        s_debugBufferObj = vbo;
        s_debugBuffer = vao;
    }

    static void Dispose_DebugDraw()
    {
        glDeleteProgram(s_debugShader);
        glDeleteBuffers(1, &s_debugBuffer);
    }

    void Graphics::DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color)
    {
        glm::mat4 m;
        m[0] = glm::vec4(p1, 0.0f); m[1] = color;
        m[2] = glm::vec4(p2, 0.0f); m[3] = color;
        s_debugLines.emplace_back(m);
    }

    void Graphics::DrawDebugShape(const std::vector<glm::mat4>& lines)
    {
        s_debugLines.reserve(std::max(s_debugLines.capacity(), s_debugLines.size() + lines.size()));
        s_debugLines.insert(s_debugLines.end(), lines.begin(), lines.end());
    }

    void Graphics::DrawDebugCommands(const Scene& scene)
    {
        glBindBuffer(GL_ARRAY_BUFFER, s_debugBufferObj);
        glBufferData(GL_ARRAY_BUFFER, s_debugLines.size() * sizeof(glm::mat4), s_debugLines.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(s_debugBuffer);
        
        glUseProgram(s_debugShader);
        glm::mat4 vp = scene.camera.projection * scene.camera.view;
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
        glDrawArrays(GL_LINES, 0, 2 * (GLsizei)s_debugLines.size());

        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Graphics::ClearDebugCommands()
    {
        s_debugLines.clear();
    }

    static const char* GetGlSource(GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:               return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:       return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION:       return "Application";
        case GL_DEBUG_SOURCE_OTHER:             return "Other";
        }

        return "Unknown";
    }

    static const char* GetGlType(GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
        case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
        }

        return "Unknown";
    }

    static const char* GetGlSeverity(GLenum severity)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:            return "High";
        case GL_DEBUG_SEVERITY_MEDIUM:          return "Medium";
        case GL_DEBUG_SEVERITY_LOW:             return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION:    return "Notification";
        }

        return "Unknown";
    }

    static void APIENTRY glDebugOutput(GLenum source, GLenum type, u32 id, GLenum severity, GLsizei length, const char* message, const void* userParam)
    {
        // Ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        Log::Debug("GL message ID:({}) - Source:({}) - Type:({}) - Severity:({})", id, GetGlSource(source), GetGlType(type), GetGlSeverity(severity));
        Log::Debug(message);
    }

    /*static bool Init_ImGui(void* pWindow)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        if (!ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)pWindow, true))
        {
            Log::Error("Failed to initialize ImGui GLFW backend!");
            return false;
        }

#if defined WIN32
        if (!ImGui_ImplOpenGL3_Init("#version 330 core\n"))
#elif defined __arm__
        if (!ImGui_ImplOpenGL3_Init("#version 300 es\n"))
#endif
        {
            Log::Error("Failed to initialize ImGui OpenGL backend!");
            return false;
        }

        return true;
    }*/

    static bool Init_GlDebugLog()
    {
        i32 flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            // Initialize debug output
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            
            return true;
        }
        
        return false;
    }

    static void Print_GlInfo()
    {
        i32 numOfExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numOfExtensions);
        Log::Debug("GL Supported extensions ({}):", numOfExtensions);
        for (i32 i = 0; i < numOfExtensions; i++)
        {
            Log::Debug((const char*)glGetStringi(GL_EXTENSIONS, i));
        }
    }

    bool Graphics::Initialize()
    {
        // Setup GL load proc
#if defined GL_BACKEND
        if (!gladLoadGLLoader((GLADloadproc)Window::GetLoadProc()))
#elif defined GLES_BACKEND
        if (!gladLoadGLES2Loader((GLADloadproc)Window::GetLoadProc()))
#endif
        {
            Log::Error("Failed to initialize GLAD!");
            return false;
        }

        if (!Init_GlDebugLog())
            Log::Warning("GL debug output not supported.");
        
        // Initialize ImGui
        //if (!Init_ImGui(Window::GetWindowPtr()))
        //{
        //    Log::Error("Failed to initialize ImGui!");
        //    return false;
        //}

        Print_GlInfo();

        Init_DebugDraw();

        return true;
    }

    void Graphics::Shutdown()
    {
        //ImGui_ImplOpenGL3_Shutdown();
        //ImGui_ImplGlfw_Shutdown();
        //ImGui::DestroyContext();

        Dispose_DebugDraw();
    }

    void Graphics::NewFrame()
    {
        //ImGui_ImplGlfw_NewFrame();
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui::NewFrame();
    }

    void Graphics::Render()
    {
        //DrawDebug();

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}