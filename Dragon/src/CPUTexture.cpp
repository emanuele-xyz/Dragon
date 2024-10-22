#include <Dragon/pch.h>
#include <Dragon/CPUTexture.h>
#include <Dragon/Commons.h>
#include <Dragon/Win32Utils.h>

namespace Dragon
{
    CPUTexture CPUTexture::LoadFromFile(const std::string& path)
    {
        CPUTexture texture{};
        auto wpath{ GetWStrFromStr(path) };
        Dragon_CheckHR(LoadFromWICFile(wpath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, texture));
        return texture;
    }
    CPUTexture CPUTexture::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        CPUTexture texture{};
        Dragon_CheckHR(texture.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, 64, 64, 1, 1)); // NOTE: use mipLevels=0 for auto generating the whole mipmap chain
        auto pixels{ texture.GetPixels() };
        uint8_t colors[4]{ r, g, b, a };
        for (size_t i{}; i < texture.GetPixelsSize(); i++)
        {
            pixels[i] = colors[i % 4];
        }
        return texture;
    }
}
