#include "Editor/EditorScript.hpp"
#include "Editor/ImGuiImpl.hpp"

#include "Engine/Application.hpp"
#include "Engine/Window.hpp"
#include "Engine/Graphics.hpp"
#include "Engine/Script.hpp"

#include "Engine/Resource.hpp"

using namespace vi;

static double WrenCos(double x)
{
	return cos(x);
}

static double WrenSin(double x)
{
	return sin(x);
}

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

	if (!EditorScript::Initialize())
	{
		Log::Error("Failed to initialize editor script module!");
		return false;
	}

	if (!ImGuiImpl::Initialize(Window::GetWindowPtr()))
	{
		Log::Error("Failed to initialize imgui!");
		return false;
	}

	ScriptManager::BindFunction<decltype(&WrenCos), &WrenCos>(true, "main::Math::cos(_)_s");
	ScriptManager::BindFunction<decltype(&WrenCos), &WrenCos>(true, "main::Math::sin(_)_s");

	ScriptManager::Interpret(
		"main",
		"class Math {\n"
		"  foreign static cos(num)\n"
		"  foreign static sin(num)\n"
		"}\n"
		"System.print(\"%(Math.cos(1.570796326))\")\n"
	);

	return true;
}

void Application::Shutdown()
{
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
}

void Application::Render()
{
	Graphics::Render();
	Window::Display();
}

int main(int argc, char** argv)
{
	ResourceManager::RegisterPath(EDITOR_ASSETS_PATH);

	AppConfig config;
	config.winTitle = "Editor";
	config.winWidth = 800;
	config.winHeight = 600;

	return Application::Run(config, argc, argv);
}