#include "Engine/Application.hpp"

#include "Engine/Log.hpp"
#include "Engine/Time.hpp"
#include "Engine/Profiler.hpp"

namespace vi
{
	int Application::Run(int argc, char** argv)
	{
		if (!Initialize())
			return EXIT_FAILURE;

		Timer timer;
		while (IsRunning())
		{
			float dt = timer.Elapsed();
			timer.Restart();

			Update(dt);
			Render();
		}

		Shutdown();
		return EXIT_SUCCESS;
	}
}