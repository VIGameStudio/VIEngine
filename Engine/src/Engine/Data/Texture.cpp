#include "Engine/Data/Texture.hpp"
#include "Engine/Resource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstring>

namespace vi
{
    template<>
    bool ResourceData<Texture>::LoadImpl(const std::string& filename, Texture& data)
    {
        stbi_set_flip_vertically_on_load(true);
        auto pData = stbi_load(ResourceManager::GetFilePath(filename).c_str(), &data.width, &data.height, &data.channels, 4);
        if (pData == nullptr)
            return false;

        data.pixels.resize((i64)data.width * (i64)data.height * 4);
        memcpy(data.pixels.data(), pData, data.pixels.size());
        stbi_image_free(pData);

        return true;
    }
}