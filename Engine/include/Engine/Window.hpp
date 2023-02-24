#pragma once

namespace vi
{
	class Window
	{
	public:
		static bool IsOpen();
		static void GetSize(int* width, int* height);

	private:
		friend class Application;

		static bool Create();
		static void Destroy();

		static void PollEvents();
		static void Display();

		static const void* GetWindowPtr();
		static const void* GetLoadProc();
	};
}