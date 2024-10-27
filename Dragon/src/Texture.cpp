#include <Dragon/pch.h>
#include <Dragon/Texture.h>

#include <DirectXTex.h>

namespace Dragon
{
    Texture::Texture(ID3D11Device* device, const std::filesystem::path& path)
        : m_texture{}
        , m_texture_srv{}
    {
        DirectX::ScratchImage image{};
        Dragon_CheckHR(DirectX::LoadFromWICFile(path.wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image));

        const auto metadata{ image.GetMetadata() };

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = static_cast<UINT>(metadata.width);
        desc.Height = static_cast<UINT>(metadata.height);
        desc.MipLevels = static_cast<UINT>(metadata.mipLevels);
        desc.ArraySize = static_cast<UINT>(metadata.arraySize);
        desc.Format = DirectX::MakeLinear(metadata.format);
        desc.SampleDesc = { .Count = 1, .Quality = 0 };
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subres_data{};
        subres_data.pSysMem = image.GetImage(0, 0, 0)->pixels;
        subres_data.SysMemPitch = static_cast<UINT>(image.GetImage(0, 0, 0)->rowPitch);
        subres_data.SysMemSlicePitch = static_cast<UINT>(image.GetImage(0, 0, 0)->slicePitch);

        m_texture = D3D11Utils::CreateTexture2D(device, &desc, &subres_data);
        m_texture_srv = D3D11Utils::CreateSRV(device, m_texture.Get());
    }
}