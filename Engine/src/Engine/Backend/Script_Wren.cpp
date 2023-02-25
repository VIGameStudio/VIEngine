#include "Engine/Script.hpp"

#include "Engine/Log.hpp"

#include <fmt/format.h>
#include <wren.hpp>

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace vi
{
	static WrenVM* vm = nullptr;
	static std::unordered_map<std::string, WrenForeignMethodFn> boundForeignMethods;

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

	static WrenLoadModuleResult WrenLoadModule(WrenVM* vm, const char* name)
	{
		std::string path(name);
		path += ".wren";
		std::ifstream fin;
		fin.open(path, std::ios::in);
		std::stringstream buffer;
		buffer << fin.rdbuf() << '\0';
		std::string source = buffer.str();

		char* cbuffer = (char*)malloc(source.size());
		memcpy(cbuffer, source.c_str(), source.size());

		WrenLoadModuleResult result;
		result.source = cbuffer;

		return result;
	}

	static WrenForeignMethodFn WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature)
	{
		std::string fnName = fmt::format("{}::{}::{}_{}", module, className, signature, isStatic ? "s" : "");
		auto it = boundForeignMethods.find(fnName);
		if (it != boundForeignMethods.end())
			return it->second;
		return nullptr;
	}

	void ScriptManager::BindFunctionImpl(bool isStatic, const char* name, void(*fn)(void*))
	{
		boundForeignMethods.insert(std::make_pair(name, (WrenForeignMethodFn)fn));
	}

	double ScriptManager::GetMethodArg(void* vm, int slot)
	{
		return wrenGetSlotDouble((WrenVM*)vm, slot);
	}
	
	void ScriptManager::SetMethodArg(void* vm, int slot, double val)
	{
		wrenSetSlotDouble((WrenVM*)vm, slot, val);
	}

	bool ScriptManager::Initialize()
	{
		WrenConfiguration config;
		wrenInitConfiguration(&config);

		//config.reallocateFn = defaultReallocate;
		//config.resolveModuleFn = NULL;
		config.loadModuleFn = &WrenLoadModule;
		config.bindForeignMethodFn = &WrenBindForeignMethod;
		//config.bindForeignClassFn = NULL;
		config.writeFn = &WrenWrite;
		config.errorFn = &WrenError;
		//config.initialHeapSize = 1024 * 1024 * 10;
		//config.minHeapSize = 1024 * 1024;
		//config.heapGrowthPercent = 50;
		//config.userData = NULL;

		vm = wrenNewVM(&config);
		if (vm == nullptr)
		{
			Log::Error("Failed to create Wren VM!");
		}

		return true;
	}

	void ScriptManager::Shutdown()
	{
		wrenFreeVM(vm);
	}

	bool ScriptManager::Interpret(const char* module, const char* source)
	{
		switch (wrenInterpret(vm, module, source))
		{
		case WREN_RESULT_COMPILE_ERROR:
			Log::Error("Compile Error!"); break;
		case WREN_RESULT_RUNTIME_ERROR:
			Log::Error("Runtime Error!"); break;
		case WREN_RESULT_SUCCESS:
			return true;
		}
		return false;
	}
}