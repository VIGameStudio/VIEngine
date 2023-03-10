#include "Editor/ImGui.hpp"
#include "Editor/EditorScript.hpp"
#include "Editor/Test.hpp"

#include "Engine/Application.hpp"
#include "Engine/Window.hpp"
#include "Engine/Graphics.hpp"
#include "Engine/Script.hpp"

#include "Engine/Resource.hpp"

using namespace vi;

bool Application::Initialize(const AppConfig& config)
{
	if (!Window::Create(config.winTitle, config.winWidth, config.winHeight, config.winFullscreen, config.winVSync))
	{
		Log::Error("Failed to create window!");
		return false;
	}

	if (!Graphics::Initialize(Window::GetLoadProc()))
	{
		Log::Error("Failed to initialize graphics!");
		return false;
	}

	if (!ScriptManager::Initialize())
	{
		Log::Error("Failed to initialize script manager!");
		return false;
	}

	if (!ImGuiImpl::Initialize(Window::GetWindowPtr()))
	{
		Log::Error("Failed to initialize imgui!");
		return false;
	}

	//if (!EditorScript::Initialize())
	//{
	//	Log::Error("Failed to initialize editor script module!");
	//	return false;
	//}

	if (!Test::Initialize())
	{
		Log::Error("Failed to initialize test!");
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	Test::Shutdown();

	ImGuiImpl::Shutdown();

	ScriptManager::Shutdown();
	Graphics::Shutdown();
	Window::Destroy();
}

bool Application::IsRunning()
{
	return Window::IsOpen();
}

void Application::Update(float dt)
{
	Window::PollEvents();
	ImGuiImpl::NewFrame();

	//EditorScript::Update(dt);
	Test::Update(dt);
}

void Application::Render()
{
	Graphics::SetClearColor(0, 0, 0, 1);
	Graphics::ClearScreen(1, 1, 1);

	Test::Render();

	Graphics::Render();
	ImGuiImpl::Render();
	Window::Display();
}

int main(int argc, char** argv)
{
	ResourceManager::RegisterPath(EDITOR_ASSETS_PATH);

	AppConfig config;
	config.winTitle = "Editor";
	config.winWidth = 800;
	config.winHeight = 600;

	int ret = Application::Run(config, argc, argv);
	ASSERT(ret == EXIT_SUCCESS);
	return ret;
}