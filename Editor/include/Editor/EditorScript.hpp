#pragma once

namespace vi
{
	class EditorScript
	{
	public:
		static bool Initialize();
		static void Shutdown();

		static void Update(float dt);

	private:
	};
}