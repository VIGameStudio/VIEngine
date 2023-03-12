#pragma once

namespace vi
{
	class Test
	{
	public:
		static bool Initialize();
		static void Shutdown();

		static void Update(float dt);
		static void Render();
	};
}