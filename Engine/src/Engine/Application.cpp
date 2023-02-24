#include "Engine/Application.hpp"

#include "Engine/Log.hpp"
#include "Engine/Graphics.hpp"
#include "Engine/Window.hpp"
#include "Engine/Level.hpp"
#include "Engine/Time.hpp"
#include "Engine/Profiler.hpp"

namespace vi
{
	static Level* pCurrentLevel = nullptr;
	static Level* pNextLevel = nullptr;

	void Application::SetLevel(Level* pLevel)
	{
		if (pNextLevel != nullptr)
		{
			Log::Warning("Next level was already set, overwriting previous.");
			delete pNextLevel;
		}

		pNextLevel = pLevel;
	}

	int Application::Run(int argc, char** argv)
	{
		if (!Initialize())
			return EXIT_FAILURE;

		Timer timer;
		while (Window::IsOpen())
		{
			float dt = timer.Elapsed();
			timer.Restart();

			Update(dt);
			Render();
		}

		Shutdown();
		return EXIT_SUCCESS;
	}

	bool Application::Initialize()
	{
		if (!Window::Create())
		{
			Log::Error("Failed to create window!");
			return false;
		}

		if (!Graphics::Initialize())
		{
			Log::Error("Failed to initialize graphics!");
			return false;
		}

		return true;
	}

	void Application::Shutdown()
	{
		Graphics::Shutdown();
		Window::Destroy();
	}

	void Application::Update(float dt)
	{
		if (pNextLevel != nullptr)
		{
			if (pCurrentLevel != nullptr)
			{
				pCurrentLevel->Clean();
				delete pCurrentLevel;
			}

			pCurrentLevel = pNextLevel;
			pNextLevel = nullptr;

			pCurrentLevel->Start();
		}

		Window::PollEvents();
		Graphics::NewFrame();
		pCurrentLevel->Update(dt);
	}

	void Application::Render()
	{
		pCurrentLevel->Render();

		Graphics::Render();
		Window::Display();
	}
}