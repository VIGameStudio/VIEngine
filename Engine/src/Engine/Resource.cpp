#include "Engine/Resource.hpp"

#include <sys/stat.h>

namespace vi
{
    static std::vector<std::string> m_paths;

    void ResourceManager::RegisterPath(const std::string& path)
    {
        m_paths.emplace_back(path);
    }

    static bool Exists(const std::string& filepath)
    {
        struct stat buffer;
        return (stat(filepath.c_str(), &buffer) == 0);
    }

    std::string ResourceManager::GetFilePath(const std::string& filename)
    {
        for (const auto& path : m_paths)
        {
            auto filepath = path + filename;
            if (Exists(filepath))
                return filepath;
        }

        ASSERT(false && "Can't find file!");
    }
}