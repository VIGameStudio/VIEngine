#include "Editor/EditorScript.hpp"

#include "Engine/Resource.hpp"
#include "Engine/Script.hpp"

namespace vi
{
	static double WrenCos(double x)
	{
		return cos(x);
	}

	static double WrenSin(double x)
	{
		return sin(x);
	}

	bool EditorScript::Initialize()
	{
		ScriptManager::BindFunction<decltype(&WrenCos), &WrenCos>(true, "main::Math::cos(_)_s");
		ScriptManager::BindFunction<decltype(&WrenCos), &WrenCos>(true, "main::Math::sin(_)_s");

		Resource<Script> res = Resource<Script>("/Editor/Scripts/Test.wren");
		if (!res.Load()) return false;
		auto& data = res.GetData();

		ScriptManager::Interpret("main", data.source.c_str());

		return true;
	}

	void EditorScript::Shutdown()
	{
	}
}