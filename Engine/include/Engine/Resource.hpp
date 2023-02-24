#pragma once

#include "Engine/Assert.hpp"
#include "Engine/Log.hpp"
#include "Engine/Math.hpp"

//#include <ctti/type_id.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace vi
{
	class ResourceManager
	{
	public:
		static void RegisterPath(const std::string& path);
		static std::string GetFilePath(const std::string& filename);
	};

	template <typename TData>
	class ResourceData
	{
	private:
		template <typename TData2> friend class Resource;

		static std::unordered_map<std::size_t, TData>& GetDataMap()
		{
			static std::unordered_map<std::size_t, TData> s_dataMap;
			return s_dataMap;
		}

		static bool HasData(std::size_t hash)
		{
			auto& dataMap = GetDataMap();
			return dataMap.find(hash) != dataMap.end();
		}

		static const TData& GetData(std::size_t hash)
		{
			auto& dataMap = GetDataMap();

			auto it = dataMap.find(hash);
			ASSERT(it != dataMap.end() && "Map does not contain data!");
			return it->second;
		}

		static bool Load(std::size_t hash, const std::string& filename)
		{
			auto& dataMap = GetDataMap();

			auto it = dataMap.find(hash);
			if (it != dataMap.end())
				return true;

			TData data;
			if (!LoadImpl(filename, data))
				return false;

			dataMap.insert(std::make_pair(hash, data));
			return true;
		}

		static bool Unload(std::size_t hash, const std::string& filename)
		{
			auto& dataMap = GetDataMap();

			auto it = dataMap.find(hash);
			if (it == dataMap.end())
				return false;

			dataMap.erase(it);

			return true;
		}

		static bool LoadImpl(const std::string& filename, TData& data);
	};

	class IResource
	{
	public:
		virtual const std::string& GetFilename() const = 0;
		virtual std::size_t GetHash() const = 0;

		virtual bool IsValid() const = 0;
		virtual bool HasData() const = 0;

		virtual bool Load() const = 0;
		virtual bool Unload() const = 0;
	};

	template <typename TData>
	class Resource : public IResource
	{
	public:
		Resource() { }
		Resource(const std::string& filename)
			: m_filename(filename)
		{
			static const std::hash<std::string> s_stringHashFn;
			m_hash = s_stringHashFn(filename);//^ ctti::type_id<TData>().hash();
		}
		~Resource() {}

		inline const std::string& GetFilename() const override
		{
			return m_filename;
		}

		inline std::size_t GetHash() const override
		{
			return m_hash;
		}

		inline bool IsValid() const override
		{
			return m_hash != 0 && !m_filename.empty();
		}

		inline bool HasData() const override
		{
			return ResourceData<TData>::HasData(m_hash);
		}

		inline const TData& GetData() const
		{
			return ResourceData<TData>::GetData(m_hash);
		}

		inline bool Load() const override
		{
			if (!ResourceData<TData>::Load(m_hash, m_filename))
			{
				Log::Warning("Failed to load resource data: {}", m_filename);
				return false;
			}
			return true;
		}

		inline bool Unload() const override
		{
			if (!ResourceData<TData>::Unload(m_hash, m_filename))
			{
				Log::Warning("Failed to unload resource data: {}", m_filename);
				return false;
			}
			return true;
		}

		inline bool operator==(const Resource& r) { return m_hash == r.m_hash; }
		inline bool operator!=(const Resource& r) { return m_hash != r.m_hash; }

	private:
		std::string m_filename;
		std::size_t m_hash = 0;
	};
}