#pragma once

#include <Dragon/D3D11Utils.h>

#include <vector>
#include <string>

namespace Dragon
{
    struct GfxSettings
    {
        GfxSettings(ID3D11Device* device);

        bool vsync;
        size_t anisotropy_index;
        std::vector<unsigned> anisotropy_levels;
        std::vector<std::string> anisotropy_descriptions;
        size_t msaa_index;
        std::vector<unsigned> msaa_samples;
        std::vector<std::string> msaa_descriptions;
    };
}
