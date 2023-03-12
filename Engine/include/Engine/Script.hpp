#pragma once

#include <string>

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
	};
}