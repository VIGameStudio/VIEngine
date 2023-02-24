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
		friend class Graphics;

		static bool Create();
		static void Destroy();

		static void* GetWindowPtr();
		static void* GetLoadProc();

		static void PollEvents();
		static void Display();
	};
}