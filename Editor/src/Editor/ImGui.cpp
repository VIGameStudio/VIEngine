#include "Editor/ImGui.hpp"

#include "Engine/Log.hpp"
#include "Engine/Window.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace DearImGui = ImGui;

#define DEARIMGUI_CHECKVERSION() DearImGui::DebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))

namespace vi
{
	bool ImGui::Initialize(const void* pWindow)
	{
        // Setup Dear ImGui context
        DEARIMGUI_CHECKVERSION();
        DearImGui::CreateContext();
        ImGuiIO& io = DearImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        DearImGui::StyleColorsDark();
        //DearImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = DearImGui::GetStyle();
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
	}

	void ImGui::Shutdown()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        DearImGui::DestroyContext();
	}

	void ImGui::NewFrame()
	{
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        DearImGui::NewFrame();
	}

	void ImGui::Render()
	{
        DearImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(DearImGui::GetDrawData());
	}

    bool ImGui::Begin(const char* name)
    {
        return DearImGui::Begin(name);
    }

    void ImGui::End()
    {
        DearImGui::End();
    }

    bool ImGui::Button(const char* label, float w, float h)
    {
        return DearImGui::Button(label, ImVec2(w, h));
    }

    float ImGui::DragFloat(const char* label, float v)
    {
        DearImGui::DragFloat(label, &v);
        return v;
    }

    float ImGui::SliderFloat(const char* label, float v, float v_min, float v_max)
    {
        DearImGui::SliderFloat(label, &v, v_min, v_max);
        return v;
    }

    bool ImGui::BeginMenuBar()
    {
        return DearImGui::BeginMenuBar();
    }

    void ImGui::EndMenuBar()
    {
        DearImGui::EndMenuBar();
    }

    bool ImGui::BeginMainMenuBar()
    {
        return DearImGui::BeginMainMenuBar();
    }

    void ImGui::EndMainMenuBar()
    {
        DearImGui::EndMainMenuBar();
    }

    bool ImGui::BeginMenu(const char* label)
    {
        return DearImGui::BeginMenu(label);
    }

    void ImGui::EndMenu()
    {
        DearImGui::EndMenu();
    }

    bool ImGui::MenuItem(const char* label)
    {
        return DearImGui::MenuItem(label);
    }

    void ImGui::ShowDemoWindow()
    {
        DearImGui::ShowDemoWindow();
    }
}