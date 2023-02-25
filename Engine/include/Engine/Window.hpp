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

		static bool Create(const char* title, int width, int height, bool fullscreen, bool vsync);
		static void Destroy();

		static void PollEvents();
		static void Display();

		static const void* GetWindowPtr();
		static const void* GetLoadProc();
	};
}