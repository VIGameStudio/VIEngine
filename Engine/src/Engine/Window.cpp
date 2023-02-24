#include "Engine/Window.hpp"

#include "Engine/Log.hpp"

#include <GLFW/glfw3.h>

#include <stdlib.h>

namespace vi
{
	static GLFWwindow* pWindow = nullptr;

	bool Window::IsOpen()
	{
		return !glfwWindowShouldClose(pWindow);
	}

	void Window::GetSize(int* width, int* height)
	{
		glfwGetFramebufferSize(pWindow, width, height);
	}

	static void glfw_error_callback(int i, const char* c)
	{
		Log::Error("GLFW ({}) {}", i, c);
	}

	bool Window::Create()
	{
#ifdef __arm__
		if (putenv((char*)"DISPLAY=:0"))
		{
			Log::Error("Failed to set DISPLAY enviroment variable!");
			return false;
		}
#endif

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			Log::Error("Failed to initialize GLFW!");
			return false;
		}

#ifdef WIN32
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		glfwWindowHint(GLFW_SAMPLES, 8);

		pWindow = glfwCreateWindow(800, 600, "Gauntlet", NULL, NULL);
#endif

#ifdef __arm__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		//glfwWindowHint(GLFW_SAMPLES, 2);

		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);

		glfwWindowHint(GLFW_RED_BITS, pMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, pMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, pMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, pMode->refreshRate);

		pWindow = glfwCreateWindow(pMode->width, pMode->height, "Gauntlet", pMonitor, NULL);
#endif

		if (pWindow == NULL)
		{
			Log::Error("Failed to create GLFW window!");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(pWindow);
		glfwSwapInterval(1);

		return true;
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(pWindow);
		glfwTerminate();
	}

	void* Window::GetWindowPtr()
	{
		return (void*)pWindow;
	}

	void* Window::GetLoadProc()
	{
		return (void*)glfwGetProcAddress;
	}

	void Window::PollEvents()
	{
		glfwPollEvents();

		if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow, true);
	}

	void Window::Display()
	{
		glfwSwapBuffers(pWindow);
	}
}