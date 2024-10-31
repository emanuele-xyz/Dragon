#include <Dragon/pch.h>
#include <Dragon/Texture.h>

#include <DirectXTex.h>

namespace Dragon
{
    Texture::Texture(ID3D11Device* device, const std::filesystem::path& path)
        : m_texture{}
        , m_texture_srv{}
    {
        DirectX::ScratchImage mip_chain{};
        {
            DirectX::ScratchImage img{};
            Dragon_CheckHR(DirectX::LoadFromWICFile(path.wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, img));
            Dragon_CheckHR(DirectX::GenerateMipMaps(*img.GetImage(0, 0, 0), DirectX::TEX_FILTER_DEFAULT, 0, mip_chain, false));
        }

        const auto metadata{ mip_chain.GetMetadata() };

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

        auto subres_data{ std::make_unique<D3D11_SUBRESOURCE_DATA[]>(metadata.mipLevels) };
        for (size_t mip_level{}; mip_level < metadata.mipLevels; mip_level++)
        {
            auto img{ mip_chain.GetImage(mip_level, 0, 0) };
            subres_data[mip_level].pSysMem = img->pixels;
            subres_data[mip_level].SysMemPitch = static_cast<UINT>(img->rowPitch);
            subres_data[mip_level].SysMemSlicePitch = static_cast<UINT>(img->slicePitch);
        }

        m_texture = D3D11Utils::CreateTexture2D(device, &desc, subres_data.get());
        m_texture_srv = D3D11Utils::CreateSRV(device, m_texture.Get());
    }
}