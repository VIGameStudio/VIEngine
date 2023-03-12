#pragma once

namespace vi
{
	class Configuration
	{
	public:
		static void* Get(const char* name);
		static void Set(const char* name, void* val);
	};
}