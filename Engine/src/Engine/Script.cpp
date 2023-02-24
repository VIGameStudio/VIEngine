#include "Engine/Script.hpp"

#include <wren.hpp>
#include <stdio.h>

namespace vi
{
	static WrenVM* vm = nullptr;

	static void WrenWrite(WrenVM* vm, const char* text)
	{
		printf("%s", text);
	}

	static void WrenError(WrenVM* vm, WrenErrorType errorType, const char* module, const int line, const char* msg)
	{
		switch (errorType)
		{
		case WREN_ERROR_COMPILE:
			printf("[%s line %d] [Error] %s\n", module, line, msg);
			break;

		case WREN_ERROR_STACK_TRACE:
			printf("[%s line %d] in %s\n", module, line, msg);
			break;

		case WREN_ERROR_RUNTIME:
			printf("[Runtime Error] %s\n", msg);
			break;
		}
	}

	bool Script::Initialize()
	{
		WrenConfiguration config;
		wrenInitConfiguration(&config);
		config.writeFn = &WrenWrite;
		config.errorFn = &WrenError;
		vm = wrenNewVM(&config);

		const char* module = "main";
		const char* script = "System.print(\"I am running in a VM!\")";

		WrenInterpretResult result = wrenInterpret(vm, module, script);

		//switch (result)
		//{
		//case WREN_RESULT_COMPILE_ERROR:
		//{ printf("Compile Error!\n"); } break;
		//case WREN_RESULT_RUNTIME_ERROR:
		//{ printf("Runtime Error!\n"); } break;
		//case WREN_RESULT_SUCCESS:
		//{ printf("Success!\n"); } break;
		//}

		return true;
	}

	void Script::Shutdown()
	{
		wrenFreeVM(vm);
	}
}