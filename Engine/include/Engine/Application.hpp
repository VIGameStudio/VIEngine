#pragma once

#include "Engine/Utils.hpp"

namespace Gaun
{
	class Application
	{
	public:
		static void SetLevel(class Level* pLevel);
		static int Run(int argc, char** argv);

	private:
		static bool Initialize();
		static void Shutdown();

		static void Update(f32 dt);
		static void Render();
	};
}