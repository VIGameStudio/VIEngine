#pragma once

namespace vi
{
	class Application
	{
	public:
		static void SetLevel(class Level* pLevel);
		static int Run(int argc, char** argv);

	private:
		static bool Initialize();
		static void Shutdown();

		static void Update(float dt);
		static void Render();
	};
}