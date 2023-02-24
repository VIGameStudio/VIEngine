#include "Engine/Resource.hpp"

namespace vi
{
    std::string ResourceManager::GetFilePath(const std::string& filename)
    {
        return ASSETS_PATH + filename;
    }
}