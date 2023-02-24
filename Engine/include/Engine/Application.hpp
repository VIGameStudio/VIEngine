#pragma once

namespace vi
{
	class Application
	{
	public:
		static int Run(int argc, char** argv);

	private:
		static bool Initialize();
		static void Shutdown();

		static bool IsRunning();

		static void Update(float dt);
		static void Render();
	};
}