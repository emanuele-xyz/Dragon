#pragma once

#include <Dragon/D3D11Utils.h>

#include <filesystem>

namespace Dragon
{
	class Texture
	{
	public:
		Texture(ID3D11Device* device, const std::filesystem::path& path);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) noexcept = delete;
	public:
		ID3D11ShaderResourceView* GetSRV() { return m_texture_srv.Get(); }
		ID3D11ShaderResourceView** GetAddressOfSRV() { return m_texture_srv.GetAddressOf(); }
	private:
		wrl::ComPtr<ID3D11Texture2D> m_texture;
		wrl::ComPtr<ID3D11ShaderResourceView> m_texture_srv;
	};
}
