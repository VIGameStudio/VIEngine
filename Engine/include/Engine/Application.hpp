#pragma once

namespace vi
{
	struct AppConfig
	{
		const char* winTitle = nullptr;
		int winWidth = 0;
		int winHeight = 0;
		bool winFullscreen = false;
		bool winVSync = true;
	};

	class Application
	{
	public:
		static int Run(const AppConfig& config, int argc, char** argv);

	private:
		static bool Initialize(const AppConfig& config);
		static void Shutdown();

		static bool IsRunning();

		static void Update(float dt);
		static void Render();
	};
}