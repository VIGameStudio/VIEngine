#include "Engine/Script.hpp"

#include "Engine/Log.hpp"
#include "Engine/Resource.hpp"

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

	static void WrenLoadModuleComplete(WrenVM* vm, const char* name, WrenLoadModuleResult result)
	{
	}

	static WrenLoadModuleResult WrenLoadModule(WrenVM* vm, const char* name)
	{
		Resource<Script> script = Resource<Script>(name);
		if (!script.Load())
		{
			ASSERT(false && "TODO: Handle this!");
		}

		WrenLoadModuleResult result;
		result.source = script.GetData().source.c_str();
		result.onComplete = &WrenLoadModuleComplete;
		result.userData = nullptr;

		return result;
	}

	static WrenForeignMethodFn WrenBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature)
	{
		std::string fnName = fmt::format("{}::{}::{}{}", module, className, signature, isStatic ? "_s" : "");
		auto it = boundForeignMethods.find(fnName);
		if (it != boundForeignMethods.end())
			return it->second;
		return nullptr;
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

	ScriptHandle ScriptManager::MakeCallHandle(const char* signature)
	{
		return wrenMakeCallHandle(vm, signature);
	}
	
	bool ScriptManager::Call(ScriptHandle method)
	{
		WrenInterpretResult res = wrenCall(vm, (WrenHandle*)method);
		switch (res)
		{
		case WrenInterpretResult::WREN_RESULT_COMPILE_ERROR:
			Log::Error("Compiler error!");
			return false;

		case WrenInterpretResult::WREN_RESULT_RUNTIME_ERROR:
			Log::Error("Runtime error!");
			return false;
		}

		return true;
	}

	void ScriptManager::ReleaseHandle(ScriptHandle handle)
	{
		wrenReleaseHandle(vm, (WrenHandle*)handle);
	}

	int ScriptManager::GetSlotCount()
	{
		return wrenGetSlotCount(vm);
	}

	void ScriptManager::EnsureSlots(int numSlots)
	{
		wrenEnsureSlots(vm, numSlots);
	}

	//WrenType ScriptManager::GetSlotType(int slot);

	bool ScriptManager::GetSlotBool(int slot)
	{
		return wrenGetSlotCount(vm);
	}

	const char* ScriptManager::GetSlotBytes(int slot, int* length)
	{
		return wrenGetSlotBytes(vm, slot, length);
	}

	double ScriptManager::GetSlotDouble(int slot)
	{
		return wrenGetSlotDouble(vm, slot);
	}

	void* ScriptManager::GetSlotForeign(int slot)
	{
		return wrenGetSlotForeign(vm, slot);
	}

	const char* ScriptManager::GetSlotString(int slot)
	{
		return wrenGetSlotString(vm, slot);
	}

	ScriptHandle ScriptManager::GetSlotHandle(int slot)
	{
		return wrenGetSlotHandle(vm, slot);
	}

	void ScriptManager::SetSlotBool(int slot, bool value)
	{
		wrenSetSlotBool(vm, slot, value);
	}

	void ScriptManager::SetSlotBytes(int slot, const char* bytes, size_t length)
	{
		wrenSetSlotBytes(vm, slot, bytes, length);
	}

	void ScriptManager::SetSlotDouble(int slot, double value)
	{
		wrenSetSlotDouble(vm, slot, value);
	}

	void* ScriptManager::SetSlotNewForeign(int slot, int classSlot, size_t size)
	{
		return wrenSetSlotNewForeign(vm, slot, classSlot, size);
	}

	void ScriptManager::SetSlotNewList(int slot)
	{
		wrenSetSlotNewList(vm, slot);
	}

	void ScriptManager::SetSlotNewMap(int slot)
	{
		wrenSetSlotNewMap(vm, slot);
	}

	void ScriptManager::SetSlotNull(int slot)
	{
		wrenSetSlotNull(vm, slot);
	}

	void ScriptManager::SetSlotString(int slot, const char* text)
	{
		wrenSetSlotString(vm, slot, text);
	}

	void ScriptManager::SetSlotHandle(int slot, ScriptHandle handle)
	{
		wrenSetSlotHandle(vm, slot, (WrenHandle*)handle);
	}

	int ScriptManager::GetListCount(int slot)
	{
		return wrenGetListCount(vm, slot);
	}

	void ScriptManager::GetListElement(int listSlot, int index, int elementSlot)
	{
		wrenGetListElement(vm, listSlot, index, elementSlot);
	}

	void ScriptManager::SetListElement(int listSlot, int index, int elementSlot)
	{
		wrenSetListElement(vm, listSlot, index, elementSlot);
	}

	void ScriptManager::InsertInList(int listSlot, int index, int elementSlot)
	{
		wrenInsertInList(vm, listSlot, index, elementSlot);
	}

	int ScriptManager::GetMapCount(int slot)
	{
		return wrenGetMapCount(vm, slot);
	}

	bool ScriptManager::GetMapContainsKey(int mapSlot, int keySlot)
	{
		return wrenGetMapContainsKey(vm, mapSlot, keySlot);
	}

	void ScriptManager::GetMapValue(int mapSlot, int keySlot, int valueSlot)
	{
		wrenGetMapValue(vm, mapSlot, keySlot, valueSlot);
	}

	void ScriptManager::SetMapValue(int mapSlot, int keySlot, int valueSlot)
	{
		wrenSetMapValue(vm, mapSlot, keySlot, valueSlot);
	}

	void ScriptManager::RemoveMapValue(int mapSlot, int keySlot, int removedValueSlot)
	{
		wrenRemoveMapValue(vm, mapSlot, keySlot, removedValueSlot);
	}

	void ScriptManager::GetVariable(const char* module, const char* name, int slot)
	{
		wrenGetVariable(vm, module, name, slot);
	}

	bool ScriptManager::HasVariable(const char* module, const char* name)
	{
		return wrenHasVariable(vm, module, name);
	}

	bool ScriptManager::HasModule(const char* module)
	{
		return wrenHasModule(vm, module);
	}

	void ScriptManager::AbortFiber(int slot)
	{
		wrenAbortFiber(vm, slot);
	}

	void* ScriptManager::GetUserData()
	{
		return wrenGetUserData(vm);
	}

	void ScriptManager::SetUserData(void* userData)
	{
		wrenSetUserData(vm, userData);
	}

	void ScriptManager::BindFunctionImpl(bool isStatic, const char* name, void(*fn)(void*))
	{
		boundForeignMethods.insert(std::make_pair(name, (WrenForeignMethodFn)fn));
	}

	float ScriptSlot<float>::Get(void* vm, int slot)
	{
		return (float)wrenGetSlotDouble((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<float>::Set(void* vm, int slot, float val)
	{
		wrenSetSlotDouble((WrenVM*)vm, slot, (double)val);
	}

	double ScriptSlot<double>::Get(void* vm, int slot)
	{
		return wrenGetSlotDouble((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<double>::Set(void* vm, int slot, double val)
	{
		wrenSetSlotDouble((WrenVM*)vm, slot, val);
	}
	
	int ScriptSlot<int>::Get(void* vm, int slot)
	{
		auto a = wrenGetSlotForeign((WrenVM*)vm, slot);
		return (int)wrenGetSlotDouble((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<int>::Set(void* vm, int slot, int val)
	{
		wrenSetSlotDouble((WrenVM*)vm, slot, (double)val);
	}
	
	unsigned ScriptSlot<unsigned>::Get(void* vm, int slot)
	{
		return (unsigned)wrenGetSlotDouble((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<unsigned>::Set(void* vm, int slot, unsigned val)
	{
		wrenSetSlotDouble((WrenVM*)vm, slot, (double)val);
	}

	bool ScriptSlot<bool>::Get(void* vm, int slot)
	{
		return wrenGetSlotBool((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<bool>::Set(void* vm, int slot, bool val)
	{
		wrenSetSlotBool((WrenVM*)vm, slot, val);
	}

	const char* ScriptSlot<const char*>::Get(void* vm, int slot)
	{
		return wrenGetSlotString((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<const char*>::Set(void* vm, int slot, const char* val)
	{
		wrenSetSlotString((WrenVM*)vm, slot, val);
	}

	std::string ScriptSlot<std::string>::Get(void* vm, int slot)
	{
		return (std::string)wrenGetSlotString((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<std::string>::Set(void* vm, int slot, const std::string& val)
	{
		wrenSetSlotString((WrenVM*)vm, slot, val.c_str());
	}

	const char* ScriptSlot<const std::string&>::Get(void* vm, int slot)
	{
		return wrenGetSlotString((WrenVM*)vm, slot);
	}
	
	void ScriptSlot<const std::string&>::Set(void* vm, int slot, const std::string& val)
	{
		wrenSetSlotString((WrenVM*)vm, slot, val.c_str());
	}
}