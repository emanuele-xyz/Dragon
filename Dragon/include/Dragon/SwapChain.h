#pragma once

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
	class SwapChain
	{
	public:
		SwapChain(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, unsigned msaa_sample_count);
		~SwapChain() = default;
		SwapChain(const SwapChain&) = delete;
		SwapChain(SwapChain&&) noexcept = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		SwapChain& operator=(SwapChain&&) noexcept = delete;
	public:
		void SetMSAASampleCount(unsigned msaa_sample_count);
		void Resize();
		void Present(bool vsync, unsigned msaa_sample_count);
	private:
		void CreateRTVAndDSV(unsigned msaa_sample_count);
	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		wrl::ComPtr<IDXGISwapChain1> m_swap_chain;
		wrl::ComPtr<ID3D11RenderTargetView> m_rtv;
		wrl::ComPtr<ID3D11DepthStencilView> m_dsv;
	};
}
