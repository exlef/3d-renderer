#pragma once


#include <iostream>
#include <string>

#include "stb_image.hpp"
#include "config.hpp"

namespace ex
{
    class Texture
    {
    private:
        uint32_t m_textureID;
        bool m_is_srgb = APPLY_GAMMA_CRRC;

    public:
        inline uint32_t id() const { return m_textureID; }
        Texture(const std::string& path, bool is_srgb = APPLY_GAMMA_CRRC);
        ~Texture();
    };

} // namespace ex