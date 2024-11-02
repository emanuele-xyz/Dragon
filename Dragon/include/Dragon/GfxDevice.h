#pragma once

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
    class GfxDevice
    {
    public:
        GfxDevice();
        ~GfxDevice() = default;
        GfxDevice(const GfxDevice&) = delete;
        GfxDevice(GfxDevice&&) noexcept = delete;
        GfxDevice& operator=(const GfxDevice&) = delete;
        GfxDevice& operator=(GfxDevice&&) noexcept = delete;
    public:
        ID3D11Device* GetDevice() { return m_device.Get(); }
        ID3D11DeviceContext* GetContext() { return m_context.Get(); }
    private:
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<ID3D11DeviceContext> m_context;
    };
}
