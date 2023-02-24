#include "Engine/Data/Shader.hpp"
#include "Engine/Resource.hpp"

#include <fstream>
#include <sstream>

namespace vi
{
    template<>
    bool ResourceData<Shader>::LoadImpl(const std::string& filename, Shader& data)
    {
        std::ifstream is(ResourceManager::GetFilePath(filename));
        if (is.fail())
            return false;

        std::stringstream ss;
        ss << is.rdbuf();

        data.text = ss.str();
        return true;
    }
}