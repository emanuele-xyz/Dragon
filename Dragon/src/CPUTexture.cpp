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
}
