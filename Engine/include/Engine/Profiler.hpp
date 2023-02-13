#pragma once

#include "Engine/Time.hpp"

#include <map>
#include <string>

namespace Gaun
{
#define PROFILE(category) auto p##__LINE__ = ProfileEntry(category, __FUNCTION__)

	struct ProfileEntry
	{
		ProfileEntry(const char* category, const char* name);
		~ProfileEntry();

		const char* category = nullptr;
		const char* name = nullptr;
	};

	struct ProfileData
	{
		Timer timer;

		std::size_t depth = 0;
		float timestamp = 0;
	};

	struct ProfileKey
	{
		ProfileKey(const char* name, std::size_t order = 0)
			: name(name)
			, order(order)
		{}

		inline bool operator==(const ProfileKey& other) const { return name == other.name; }
		inline bool operator<(const ProfileKey& other) const { return name < other.name; }
		//inline bool operator<(const ProfileKey& other) const { return order < other.order; }

		std::string name;
		std::size_t order = 0;
	};

	class Profiler
	{
	public:
		static void Start(const char* category, const char* name);
		static void Stop(const char* category, const char* name);

		static void Clear();

		static const std::map<std::string, float>& GetSystemProfiles();
		static const std::map<ProfileKey, ProfileData>& GetFunctionProfiles();
	};
}