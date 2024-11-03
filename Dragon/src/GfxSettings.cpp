#include <Dragon/pch.h>
#include <Dragon/GfxSettings.h>

#include <Dragon/SwapChain.h>

namespace Dragon
{
    GfxSettings::GfxSettings(ID3D11Device* device)
        : vsync{}
        , anisotropy_index{}
        , anisotropy_levels{ D3D11Utils::GetAnisotropyLevels() }
        , anisotropy_descriptions{}
        , msaa_index{}
        , msaa_samples{ D3D11Utils::GetMSAASamplesForFormat(device, { DRAGON_SWAP_CHAIN_BACK_BUFFER_FORMAT, DRAGON_SWAP_CHAIN_DEPTH_STENCIL_FORMAT }) }
        , msaa_descriptions{}
    {
        for (auto level : anisotropy_levels)
        {
            if (level == 0)
            {
                anisotropy_descriptions.emplace_back("Disabled");
            }
            else
            {
                anisotropy_descriptions.emplace_back(std::format("x{}", level));
            }
        }

        for (auto sample : msaa_samples)
        {
            if (sample == 1)
            {
                msaa_descriptions.emplace_back("No MSAA");
            }
            else
            {
                msaa_descriptions.emplace_back(std::format("x{}", sample));
            }
        }
    }
}