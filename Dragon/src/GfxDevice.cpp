#include <Dragon/pch.h>
#include <Dragon/GfxDevice.h>

namespace Dragon
{
    GfxDevice::GfxDevice()
        : m_device{ D3D11Utils::CreateDevice() }
        , m_context{ D3D11Utils::GetDeviceContext(m_device.Get()) }
    {
        #if defined(_DEBUG)
        D3D11Utils::BreakOnErrors(m_device.Get());
        #endif
    }
}
