#include "Editor/ImGuiImpl.hpp"

#include "Engine/Application.hpp"
#include "Engine/Window.hpp"
#include "Engine/Graphics.hpp"
#include "Engine/Script.hpp"

#include "Engine/Resource.hpp"
#include "Engine/Resources/Shader.hpp"

#include <wren.hpp>

using namespace vi;



bool Application::Initialize()
{
	if (!Window::Create())
	{
		Log::Error("Failed to create window!");
		return false;
	}

	if (!Graphics::Initialize(Window::GetLoadProc()))
	{
		Log::Error("Failed to initialize graphics!");
		return false;
	}

	if (!Script::Initialize())
	{
		Log::Error("Failed to initialize scripting!");
		return false;
	}

	if (!ImGuiImpl::Initialize(Window::GetWindowPtr()))
	{
		Log::Error("Failed to initialize imgui!");
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	ImGuiImpl::Shutdown();

	Script::Shutdown();
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
	ResourceManager::RegisterPath(GAME_ASSETS_PATH);

	return Application::Run(argc, argv);
}