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
	struct Script
	{
		std::string source;
	};

	class ScriptManager
	{
	public:
		static bool Initialize();
		static void Shutdown();

		static bool Interpret(const char* module, const char* source);

		template<typename TFunction, TFunction fn>
		static void BindFunction(bool isStatic, const char* name);

		static double GetMethodArg(void* vm, int slot);
		static void SetMethodArg(void* vm, int slot, double val);

	private:
		static void BindFunctionImpl(bool isStatic, const char* name, void(*fn)(void*));
	};

	// WIP, heavily based on: https://github.com/Nelarius/wrenpp

	template<typename T>
	struct WrenSlotAPI
	{
		static T Get(void* vm, int slot)
		{
			//ForeignObject* obj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
			//return *static_cast<T*>(obj->objectPtr());
			return T();
		}

		static void Set(void* vm, int slot, T t)
		{
			//ForeignObjectValue<T>::setInSlot(vm, slot, t);
		}
	};

	//template<typename T>
	//struct WrenSlotAPI<T&>
	//{
	//	static T& Get(void* vm, int slot)
	//	{
	//		//ForeignObject* obj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
	//		//return *static_cast<T*>(obj->objectPtr());
	//		return T();
	//	}
	//
	//	static void Set(void* vm, int slot, T& t)
	//	{
	//		//ForeignObjectPtr<T>::setInSlot(vm, slot, &t);
	//	}
	//};
	//
	//template<typename T>
	//struct WrenSlotAPI<const T&>
	//{
	//	static const T& Get(void* vm, int slot)
	//	{
	//		//ForeignObject* obj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
	//		//return *static_cast<T*>(obj->objectPtr());
	//		return T();
	//	}
	//
	//	static void Set(void* vm, int slot, const T& t)
	//	{
	//		//ForeignObjectPtr<T>::setInSlot(vm, 0, const_cast<T*>(&t));
	//	}
	//};
	//
	//template<typename T>
	//struct WrenSlotAPI<T*>
	//{
	//	static T* Get(void* vm, int slot)
	//	{
	//		//ForeignObject* obj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
	//		//return static_cast<T*>(obj->objectPtr());
	//		return nullptr;
	//	}
	//
	//	static void Set(void* vm, int slot, T* t)
	//	{
	//		//ForeignObjectPtr<T>::setInSlot(vm, slot, t);
	//	}
	//};
	//
	//template<typename T>
	//struct WrenSlotAPI<const T*>
	//{
	//	static const T* Get(void* vm, int slot)
	//	{
	//		//ForeignObject* obj = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, slot));
	//		//return static_cast<const T*>(obj->objectPtr());
	//		return nullptr;
	//	}
	//
	//	static void Set(void* vm, int slot, const T* t)
	//	{
	//		//ForeignObjectPtr<T>::setInSlot(vm, slot, const_cast<T*>(t));
	//	}
	//};

	//template<>
	//struct WrenSlotAPI<float>
	//{
	//	static float Get(void* vm, int slot)
	//	{
	//		//return float(wrenGetSlotDouble(vm, slot));
	//		return 0;
	//	}
	//
	//	static void Set(void* vm, int slot, float val)
	//	{
	//		//wrenSetSlotDouble(vm, slot, double(val));
	//	}
	//};

	template<>
	struct WrenSlotAPI<double>
	{
		static double Get(void* vm, int slot)
		{
			//return wrenGetSlotDouble(vm, slot);
			return ScriptManager::GetMethodArg(vm, slot);
		}

		static void Set(void* vm, int slot, double val)
		{
			//wrenSetSlotDouble(vm, slot, val);
			return ScriptManager::SetMethodArg(vm, slot, val);
		}
	};

	//template<>
	//struct WrenSlotAPI<int>
	//{
	//	static int Get(void* vm, int slot)
	//	{
	//		//return int(wrenGetSlotDouble(vm, slot));
	//		return 0;
	//	}
	//
	//	static void Set(void* vm, int slot, int val)
	//	{
	//		//wrenSetSlotDouble(vm, slot, double(val));
	//	}
	//};
	//
	//template<>
	//struct WrenSlotAPI<unsigned>
	//{
	//	static unsigned Get(void* vm, int slot)
	//	{
	//		//return unsigned(wrenGetSlotDouble(vm, slot));
	//		return 0;
	//	}
	//
	//	static void Set(void* vm, int slot, unsigned val)
	//	{
	//		//wrenSetSlotDouble(vm, slot, double(val));
	//	}
	//};
	//
	//template<>
	//struct WrenSlotAPI<bool>
	//{
	//	static bool Get(void* vm, int slot)
	//	{
	//		//return wrenGetSlotBool(vm, slot);
	//		return false;
	//	}
	//
	//	static void Set(void* vm, int slot, bool val)
	//	{
	//		//wrenSetSlotBool(vm, slot, val);
	//	}
	//};
	//
	//template<>
	//struct WrenSlotAPI<const char*>
	//{
	//	static const char* Get(void* vm, int slot)
	//	{
	//		//return wrenGetSlotString(vm, slot);
	//		return "";
	//	}
	//
	//	static void Set(void* vm, int slot, const char* val)
	//	{
	//		//wrenSetSlotString(vm, slot, val);
	//	}
	//};
	//
	//template<>
	//struct WrenSlotAPI<std::string>
	//{
	//	static std::string Get(void* vm, int slot)
	//	{
	//		//return std::string(wrenGetSlotString(vm, slot));
	//		return "";
	//	}
	//
	//	static void Set(void* vm, int slot, const std::string& str)
	//	{
	//		//wrenSetSlotString(vm, slot, str.c_str());
	//	}
	//};
	//
	//template<>
	//struct WrenSlotAPI<const std::string&>
	//{
	//	static const char* Get(void* vm, int slot)
	//	{
	//		//return wrenGetSlotString(vm, slot);
	//		return "";
	//	}
	//
	//	static void Set(void* vm, int slot, const std::string& str)
	//	{
	//		//wrenSetSlotString(vm, slot, str.c_str());
	//	}
	//};

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
	//
	//template<typename R, typename... Args>
	//struct FunctionTraits<R(&)(Args...)> : public FunctionTraits<R(Args...)>
	//{
	//};
	//
	//// member function pointer
	//template<typename C, typename R, typename... Args>
	//struct FunctionTraits<R(C::*)(Args...)> : public FunctionTraits<R(Args...)>
	//{
	//};
	//
	//// const member function pointer
	//template<typename C, typename R, typename... Args>
	//struct FunctionTraits<R(C::*)(Args...) const> : public FunctionTraits<R(Args...)>
	//{
	//};

	template<typename TFunction, std::size_t... index>
	decltype(auto) InvokeHelper(void* vm, TFunction&& fn, std::index_sequence<index...>)
	{
		using Traits = FunctionTraits<std::remove_reference_t<decltype(fn)>>;
		return fn(WrenSlotAPI<typename Traits::template ArgumentType<index>>::Get(vm, index + 1)...);
	}

	template<typename TFunction>
	decltype(auto) InvokeWithArguments(void* vm, TFunction&& fn)
	{
		constexpr auto Arity = FunctionTraits<std::remove_reference_t<decltype(fn)>>::Arity;
		return InvokeHelper<TFunction>(vm, std::forward<TFunction>(fn), std::make_index_sequence<Arity>{});
	}

	//template<typename R, typename C, typename... Args, std::size_t... index>
	//decltype(auto) InvokeHelper(void* vm, R(C::* f)(Args...), std::index_sequence<index...>)
	//{
	//	//using Traits = FunctionTraits<decltype(f)>;
	//	//ForeignObject* objWrapper = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, 0));
	//	//C* obj = static_cast<C*>(objWrapper->objectPtr());
	//	//return (obj->*f)(
	//	//	WrenSlotAPI<typename Traits::template ArgumentType<index>>::get(vm, index + 1)...);
	//}

	//// const variant
	//template<typename R, typename C, typename... Args, std::size_t... index>
	//decltype(auto) InvokeHelper(void* vm, R(C::* f)(Args...) const, std::index_sequence<index...>)
	//{
	//	//using Traits = FunctionTraits<decltype(f)>;
	//	//ForeignObject* objWrapper = static_cast<ForeignObject*>(wrenGetSlotForeign(vm, 0));
	//	//const C* obj = static_cast<const C*>(objWrapper->objectPtr());
	//	//return (obj->*f)(
	//	//	WrenSlotAPI<typename Traits::template ArgumentType<index>>::get(vm, index + 1)...);
	//}

	//template<typename R, typename C, typename... Args>
	//decltype(auto) InvokeWithArguments(void* vm, R(C::* f)(Args...))
	//{
	//	//constexpr auto Arity = FunctionTraits<decltype(f)>::Arity;
	//	//return invokeHelper(vm, f, std::make_index_sequence<Arity>{});
	//}
	//
	//// const variant
	//template<typename R, typename C, typename... Args>
	//decltype(auto) InvokeWithArguments(void* vm, R(C::* f)(Args...) const)
	//{
	//	//constexpr auto Arity = FunctionTraits<decltype(f)>::Arity;
	//	//return invokeHelper(vm, f, std::make_index_sequence<Arity>{});
	//}

	// invokes plain invokeWithWrenArguments if true
	// invokes invokeWithWrenArguments within WrenReturn if false
	// to be used with std::is_void as the predicate
	template<bool predicate>
	struct InvokeWithoutReturningIf
	{
		//template<typename TFunction>
		//static void Invoke(void* vm, TFunction&& fn)
		//{
		//	//InvokeWithArguments(vm, std::forward<TFunction>(fn));
		//}
		//
		//template<typename TRet, typename TClass, typename... TArgs>
		//static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...))
		//{
		//	//InvokeWithArguments(vm, std::forward<TRet(TClass::*)(TArgs...)>(fn));
		//}
	};

	template<>
	struct InvokeWithoutReturningIf<false>
	{
		template<typename TFunction>
		static void Invoke(void* vm, TFunction&& fn)
		{
			using ReturnType = typename FunctionTraits<std::remove_reference_t<decltype(fn)>>::ReturnType;
			WrenSlotAPI<ReturnType>::Set(vm, 0, InvokeWithArguments(vm, std::forward<TFunction>(fn)));
			//Script::SetMethodArg(vm, 0, InvokeWithArguments(vm, std::forward<TFunction>(fn)));
		}

		//template<typename TRet, typename TClass, typename... TArgs>
		//static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...))
		//{
		//	//WrenSlotAPI<R>::set(vm, 0, invokeWithWrenArguments(vm, f));
		//	Script::SetMethodArg(vm, 0, 0);// InvokeWithArguments(vm, fn));
		//}
		//
		//template<typename TRet, typename TClass, typename... TArgs>
		//static void Invoke(void* vm, TRet(TClass::* fn)(TArgs...) const)
		//{
		//	//WrenSlotAPI<R>::set(vm, 0, invokeWithWrenArguments(vm, fn));
		//	Script::SetMethodArg(vm, 0, 0);//InvokeWithArguments(vm, fn));
		//}
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
	//template<typename TRet, typename TClass, typename... TArgs, TRet(TClass::* fn)(TArgs...)>
	//struct ScriptFunctionWrapper<TRet(TClass::*)(TArgs...), fn>
	//{
	//	static void Call(void* vm)
	//	{
	//		InvokeWithoutReturningIf<std::is_void<TRet>::value>::Invoke(vm, fn);
	//	}
	//};
	//
	//// const method variant
	//template<typename  TRet, typename TClass, typename... TArgs, TRet(TClass::* fn)(TArgs...) const>
	//struct ScriptFunctionWrapper<TRet(TClass::*)(TArgs...) const, fn>
	//{
	//	static void Call(void* vm)
	//	{
	//		InvokeWithoutReturningIf<std::is_void<TRet>::value>::Invoke(vm, fn);
	//	}
	//};

	template<typename TFunction, TFunction fn>
	void ScriptManager::BindFunction(bool isStatic, const char* name)
	{
		BindFunctionImpl(isStatic, name, &ScriptFunctionWrapper<decltype(fn), fn>::Call);
	}
}