#include "Engine/Script.hpp"

#include "Engine/Resource.hpp"

#include <fstream>
#include <sstream>

namespace vi
{
    template<>
    bool ResourceData<Script>::LoadImpl(const std::string& filename, Script& data)
    {
        std::ifstream is(ResourceManager::GetFilePath(filename));
        if (is.fail())
            return false;

        std::stringstream ss;
        ss << is.rdbuf();

        data.source = ss.str();
        return true;
    }
}