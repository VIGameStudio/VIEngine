#pragma once

#include <tuple>
#include <string>

//#include <functional> // for std::hash
//#include <cassert>
//#include <cstdint>
//#include <cstdlib> // for std::size_t
//#include <cstring> // for memcpy, strcpy
//#include <fstream>
//#include <sstream>
//#include <sys/stat.h>
//#include <vector>
//#include <type_traits>

namespace vi
{
	enum struct ScriptLang { UNKNOWN, LUA, WREN };

	using ScriptHandle = void*;

	struct Script
	{
		ScriptLang lang = ScriptLang::UNKNOWN;
		std::string source;
	};

	class ScriptManager
	{
	public:
		static bool Initialize();
		static void Shutdown();

		static void CollectGarbage();

		static bool Interpret(const char* module, const char* source);

		static ScriptHandle MakeCallHandle(const char* signature);
		static bool Call(ScriptHandle method);
		static void ReleaseHandle(ScriptHandle handle);

		static int GetSlotCount();
		static void EnsureSlots(int numSlots);
		//static WrenType GetSlotType(int slot);
		static bool GetSlotBool(int slot);
		static const char* GetSlotBytes(int slot, int* length);
		static double GetSlotDouble(int slot);
		static void* GetSlotForeign(int slot);
		static const char* GetSlotString(int slot);
		static ScriptHandle GetSlotHandle(int slot);
		static void SetSlotBool(int slot, bool value);
		static void SetSlotBytes(int slot, const char* bytes, size_t length);
		static void SetSlotDouble(int slot, double value);
		static void* SetSlotNewForeign(int slot, int classSlot, size_t size);
		static void SetSlotNewList(int slot);
		static void SetSlotNewMap(int slot);
		static void SetSlotNull(int slot);
		static void SetSlotString(int slot, const char* text);
		static void SetSlotHandle(int slot, ScriptHandle handle);
		static int GetListCount(int slot);
		static void GetListElement(int listSlot, int index, int elementSlot);
		static void SetListElement(int listSlot, int index, int elementSlot);
		static void InsertInList(int listSlot, int index, int elementSlot);
		static int GetMapCount(int slot);
		static bool GetMapContainsKey(int mapSlot, int keySlot);
		static void GetMapValue(int mapSlot, int keySlot, int valueSlot);
		static void SetMapValue(int mapSlot, int keySlot, int valueSlot);
		static void RemoveMapValue(int mapSlot, int keySlot, int removedValueSlot);
		static void GetVariable(const char* module, const char* name, int slot);
		static bool HasVariable(const char* module, const char* name);
		static bool HasModule(const char* module);
		static void AbortFiber(int slot);
		static void* GetUserData();
		static void SetUserData(void* userData);

		template<typename TFunction, TFunction fn>
		static void BindFunction(bool isStatic, const char* name);

		static void BindFunctionImpl(bool isStatic, const char* name, void(*fn)(void*));

	private:
		//static void BindFunctionImpl(bool isStatic, const char* name, void(*fn)(void*));
	};

	// WIP, heavily based on: https://github.com/Nelarius/wrenpp

	/*
	 * The interface for getting the object pointer. The actual C++ object may lie within the Wren
	 * object, or may live in C++.
	 */
	class ForeignObject
	{
	public:
		virtual ~ForeignObject() = default;
		virtual void* GetObjectPtr() = 0;
	};

	/*
	 * This wraps a class object by value. The lifetimes of these objects are managed in Wren.
	 */
	template<typename T>
	class ForeignObjectValue : public ForeignObject
	{
	public:
		ForeignObjectValue() : m_data() {}
		virtual ~ForeignObjectValue()
		{
			T* obj = static_cast<T*>(GetObjectPtr());
			obj->~T();
		}

		void* GetObjectPtr() override
		{
			return &m_data;
		}

		template<typename... Args>
		static void SetInSlot(void* vm, int slot, Args... arg)
		{
			//wrenEnsureSlots(vm, slot + 1);
			//wrenGetVariable(vm, getWrenModuleString<T>(), getWrenClassString<T>(), slot);
			//ForeignObjectValue<T>* val = new (wrenSetSlotNewForeign(vm, slot, slot, sizeof(ForeignObjectValue<T>))) ForeignObjectValue<T>();
			//new (val->objectPtr()) T{ std::forward<Args>(arg)... };

			ScriptManager::EnsureSlots(vm, slot + 1);
			ScriptManager::GetVariable(vm, getWrenModuleString<T>(), getWrenClassString<T>(), slot);
			ForeignObjectValue<T>* val = new (ScriptManager::SetSlotNewForeign(vm, slot, slot, sizeof(ForeignObjectValue<T>))) ForeignObjectValue<T>();
			new (val->objectPtr()) T{ std::forward<Args>(arg)... };
		}

	private:
		typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data;
	};

	/*
	 * Wraps a pointer to a class object. The lifetimes of the pointed-to objects are managed by the
	 * host program.
	 */
	template<typename T>
	class ForeignObjectPtr : public ForeignObject
	{
	public:
		explicit ForeignObjectPtr(T* pObject) : m_pObject(pObject) {}
		virtual ~ForeignObjectPtr() = default;

		void* GetObjectPtr() override
		{
			return m_pObject;
		}

		static void SetInSlot(void* vm, int slot, T* pObj)
		{
			//wrenEnsureSlots(vm, slot + 1);
			//wrenGetVariable(vm, getWrenModuleString<T>(), getWrenClassString<T>(), slot);
			//void* bytes = wrenSetSlotNewForeign(vm, slot, slot, sizeof(ForeignObjectPtr<T>));
			//new (bytes) ForeignObjectPtr<T>{obj};

			ScriptManager::EnsureSlots(vm, slot + 1);
			ScriptManager::GetVariable(vm, getWrenModuleString<T>(), getWrenClassString<T>(), slot);
			void* bytes = ScriptManager::SetSlotNewForeign(vm, slot, slot, sizeof(ForeignObjectPtr<T>));
			new (bytes) ForeignObjectPtr<T>{obj};
		}

	private:
		T* m_pObject;
	};

	template<typename T>
	struct ScriptSlot
	{
		static T Get(void* vm, int slot)
		{
			//ForeignObject* pObj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			ForeignObject* pObj = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(slot));
			return *static_cast<T*>(pObj->GetObjectPtr());
		}
	
		static void Set(void* vm, int slot, T t)
		{
			ForeignObjectValue<T>::SetInSlot(vm, slot, t);
		}
	};

	template<typename T>
	struct ScriptSlot<T&>
	{
		static T& Get(void* vm, int slot)
		{
			//ForeignObject* pObj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			ForeignObject* pObj = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(slot));
			return *static_cast<T*>(pObj->GetObjectPtr());
		}
	
		static void Set(void* vm, int slot, T& t)
		{
			ForeignObjectPtr<T>::SetInSlot(vm, slot, &t);
		}
	};
	
	template<typename T>
	struct ScriptSlot<const T&>
	{
		static const T& Get(void* vm, int slot)
		{
			//ForeignObject* pObj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			ForeignObject* pObj = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(slot));
			return *static_cast<T*>(pObj->GetObjectPtr());
		}
	
		static void Set(void* vm, int slot, const T& t)
		{
			ForeignObjectPtr<T>::SetInSlot(vm, 0, const_cast<T*>(&t));
		}
	};
	
	template<typename T>
	struct ScriptSlot<T*>
	{
		static T* Get(void* vm, int slot)
		{
			//ForeignObject* pObj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			ForeignObject* pObj = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(slot));
			return static_cast<T*>(pObj->GetObjectPtr());
		}
	
		static void Set(void* vm, int slot, T* t)
		{
			ForeignObjectPtr<T>::SetInSlot(vm, slot, t);
		}
	};
	
	template<typename T>
	struct ScriptSlot<const T*>
	{
		static const T* Get(void* vm, int slot)
		{
			//ForeignObject* pObj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			ForeignObject* pObj = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(slot));
			return static_cast<const T*>(pObj->GetObjectPtr());
		}
	
		static void Set(void* vm, int slot, const T* t)
		{
			ForeignObjectPtr<T>::SetInSlot(vm, slot, const_cast<T*>(t));
		}
	};

	template<>
	struct ScriptSlot<float>
	{
		static float Get(void* vm, int slot);
		static void Set(void* vm, int slot, float val);
	};

	template<>
	struct ScriptSlot<double>
	{
		static double Get(void* vm, int slot);
		static void Set(void* vm, int slot, double val);
	};

	template<>
	struct ScriptSlot<int>
	{
		static int Get(void* vm, int slot);
		static void Set(void* vm, int slot, int val);
	};

	template<>
	struct ScriptSlot<unsigned>
	{
		static unsigned Get(void* vm, int slot);
		static void Set(void* vm, int slot, unsigned val);
	};

	template<>
	struct ScriptSlot<bool>
	{
		static bool Get(void* vm, int slot);
		static void Set(void* vm, int slot, bool val);
	};

	template<>
	struct ScriptSlot<const char*>
	{
		static const char* Get(void* vm, int slot);
		static void Set(void* vm, int slot, const char* val);
	};

	template<>
	struct ScriptSlot<std::string>
	{
		static std::string Get(void* vm, int slot);
		static void Set(void* vm, int slot, const std::string& val);
	};

	template<>
	struct ScriptSlot<const std::string&>
	{
		static const char* Get(void* vm, int slot);
		static void Set(void* vm, int slot, const std::string& val);
	};

	template<typename F>
	struct FunctionTraits;

	template<typename R, typename... Args>
	struct FunctionTraits<R(Args...)>
	{
		using ReturnType = R;

		constexpr static const std::size_t Arity = sizeof...(Args);
		
		template<std::size_t N>
		struct Argument
		{
			static_assert(N < Arity, "FunctionTraits error: invalid argument index parameter");
			using Type = std::tuple_element_t<N, std::tuple<Args...>>;
		};
		
		template<std::size_t N>
		using ArgumentType = typename Argument<N>::Type;
	};

	template<typename R, typename... Args>
	struct FunctionTraits<R(*)(Args...)> : public FunctionTraits<R(Args...)>
	{
	};
	
	template<typename R, typename... Args>
	struct FunctionTraits<R(&)(Args...)> : public FunctionTraits<R(Args...)>
	{
	};
	
	// member function pointer
	template<typename C, typename R, typename... Args>
	struct FunctionTraits<R(C::*)(Args...)> : public FunctionTraits<R(Args...)>
	{
	};
	
	// const member function pointer
	template<typename C, typename R, typename... Args>
	struct FunctionTraits<R(C::*)(Args...) const> : public FunctionTraits<R(Args...)>
	{
	};

	template<typename TFunction, std::size_t... index>
	decltype(auto) InvokeHelper(void* vm, TFunction&& fn, std::index_sequence<index...>)
	{
		using Traits = FunctionTraits<std::remove_reference_t<decltype(fn)>>;
		return fn(ScriptSlot<typename Traits::template ArgumentType<index>>::Get(vm, index + 1)...);
	}

	template<typename TFunction>
	decltype(auto) InvokeWithArguments(void* vm, TFunction&& fn)
	{
		constexpr auto Arity = FunctionTraits<std::remove_reference_t<decltype(fn)>>::Arity;
		return InvokeHelper<TFunction>(vm, std::forward<TFunction>(fn), std::make_index_sequence<Arity>{});
	}

	template<typename TRet, typename TClass, typename... TArgs, std::size_t... index>
	decltype(auto) InvokeHelper(void* vm, TRet(TClass::* fn)(TArgs...), std::index_sequence<index...>)
	{
		using Traits = FunctionTraits<decltype(fn)>;
		//ForeignObject* pObjWrapper = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, 0));
		ForeignObject* pObjWrapper = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(0));
		TClass* pObj = static_cast<TClass*>(pObjWrapper->GetObjectPtr());
		return (pObj->*fn)(ScriptSlot<typename Traits::template ArgumentType<index>>::Get(vm, index + 1)...);
	}

	// const variant
	template<typename TRet, typename TClass, typename... TArgs, std::size_t... index>
	decltype(auto) InvokeHelper(void* vm, TRet(TClass::* fn)(TArgs...) const, std::index_sequence<index...>)
	{
		using Traits = FunctionTraits<decltype(fn)>;
		//ForeignObject* pObjWrapper = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, 0));
		ForeignObject* pObjWrapper = static_cast<ForeignObject*>(ScriptManager::GetSlotForeign(0));
		const TClass* pObj = static_cast<const TClass*>(pObjWrapper->objectPtr());
		return (pObj->*fn)(ScriptSlot<typename Traits::template ArgumentType<index>>::Get(vm, index + 1)...);
	}

	template<typename TRet, typename TClass, typename... TArgs>
	decltype(auto) InvokeWithArguments(void* vm, TRet(TClass::* fn)(TArgs...))
	{
		constexpr auto Arity = FunctionTraits<decltype(fn)>::Arity;
		return InvokeHelper(vm, fn, std::make_index_sequence<Arity>{});
	}
	
	// const variant
	template<typename TRet, typename TClass, typename... TArgs>
	decltype(auto) InvokeWithArguments(void* vm, TRet(TClass::* fn)(TArgs...) const)
	{
		constexpr auto Arity = FunctionTraits<decltype(fn)>::Arity;
		return InvokeHelper(vm, fn, std::make_index_sequence<Arity>{});
	}

	// invokes plain invokeWithWrenArguments if true
	// invokes invokeWithWrenArguments within WrenReturn if false
	// to be used with std::is_void as the predicate
	template<bool predicate>
	struct InvokeWithoutReturningIf
	{
		template<typename TFunction>
		static void Invoke(void* vm, TFunction&& fn)
		{
			InvokeWithArguments(vm, std::forward<TFunction>(fn));
		}
		
		template<typename TRet, typename TClass, typename... TArgs>
		static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...))
		{
			InvokeWithArguments(vm, std::forward<TRet(TClass::*)(TArgs...)>(fn));
		}
	};

	template<>
	struct InvokeWithoutReturningIf<false>
	{
		template<typename TFunction>
		static void Invoke(void* vm, TFunction&& fn)
		{
			using ReturnType = typename FunctionTraits<std::remove_reference_t<decltype(fn)>>::ReturnType;
			ScriptSlot<ReturnType>::Set(vm, 0, InvokeWithArguments(vm, std::forward<TFunction>(fn)));
		}

		template<typename TRet, typename TClass, typename... TArgs>
		static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...))
		{
			ScriptSlot<TRet>::Set(vm, 0, InvokeWithArguments(vm, fn));
		}
		
		template<typename TRet, typename TClass, typename... TArgs>
		static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...) const)
		{
			ScriptSlot<TRet>::Set(vm, 0, InvokeWithArguments(vm, fn));
		}
	};

	template<typename TSignature, TSignature>
	struct ScriptFunctionWrapper;

	// free function variant
	template<typename TRet, typename... TArgs, TRet(*fn)(TArgs...)>
	struct ScriptFunctionWrapper<TRet(*)(TArgs...), fn>
	{
		static void Call(void* vm)
		{
			InvokeWithoutReturningIf<std::is_void<TRet>::value>::Invoke(vm, fn);
		}
	};

	// method variant
	template<typename TRet, typename TClass, typename... TArgs, TRet(TClass::* fn)(TArgs...)>
	struct ScriptFunctionWrapper<TRet(TClass::*)(TArgs...), fn>
	{
		static void Call(void* vm)
		{
			InvokeWithoutReturningIf<std::is_void<TRet>::value>::Invoke(vm, fn);
		}
	};
	
	// const method variant
	template<typename  TRet, typename TClass, typename... TArgs, TRet(TClass::* fn)(TArgs...) const>
	struct ScriptFunctionWrapper<TRet(TClass::*)(TArgs...) const, fn>
	{
		static void Call(void* vm)
		{
			InvokeWithoutReturningIf<std::is_void<TRet>::value>::Invoke(vm, fn);
		}
	};

	template<typename TFunction, TFunction fn>
	void ScriptManager::BindFunction(bool isStatic, const char* name)
	{
		BindFunctionImpl(isStatic, name, &ScriptFunctionWrapper<decltype(fn), fn>::Call);
	}
}