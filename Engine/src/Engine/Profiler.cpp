#include "Engine/Profiler.hpp"

#include "Engine/Log.hpp"

namespace vi
{
	static std::size_t g_order = 0;
	static std::size_t g_depth = 0;

	static std::map<std::string, float> g_systemProfiles;
	static std::map<ProfileKey, ProfileData> g_functionProfiles;

	ProfileEntry::ProfileEntry(const char* category, const char* name)
		: category(category)
		, name(name)
	{
		Profiler::Start(category, name);
	}

	ProfileEntry::~ProfileEntry()
	{
		Profiler::Stop(category, name);
	}

	void Profiler::Start(const char* category, const char* name)
	{
		g_order++;

		std::string keyName = fmt::format("[{}] {}", category, name);
		ProfileKey key(keyName.c_str(), 0);

		auto& profile = g_functionProfiles[key];
		profile.depth = g_depth;
		profile.timer.Restart();

		g_depth++;
	}

	void Profiler::Stop(const char* category, const char* name)
	{
		g_depth--;

		std::string keyName = fmt::format("[{}] {}", category, name);
		ProfileKey key(keyName.c_str(), 0);

		auto& profile = g_functionProfiles[key];
		profile.timestamp = profile.timer.Elapsed();

		g_systemProfiles[category] += profile.timestamp;
	}

	void Profiler::Clear()
	{
		g_order = 0;
		g_depth = 0;

		for (auto& it : g_systemProfiles)
		{
			it.second = 0;
		}

		for (auto& it : g_functionProfiles)
		{
			it.second.timer.Restart();
			it.second.timestamp = 0;
		}
	}

	const std::map<std::string, float>& Profiler::GetSystemProfiles()
	{
		return g_systemProfiles;
	}

	const std::map<ProfileKey, ProfileData>& Profiler::GetFunctionProfiles()
	{
		return g_functionProfiles;
	}
}