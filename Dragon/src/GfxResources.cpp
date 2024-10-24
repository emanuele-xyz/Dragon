#include <Dragon/pch.h>
#include <Dragon/GfxResources.h>
#include <Dragon/ConstantBuffers.h>

namespace Dragon
{
    GfxResources::GfxResources(ID3D11Device* device)
        : m_device{ device }
    {
        // NOTE: shaders and input layout
        {
            #if defined(_DEBUG)
            #include <Dragon/hlsl/DefaultVSDebug.h> // for vs_default_bytecode
            #include <Dragon/hlsl/UnlitPSDebug.h> // for ps_unlit_bytecode
            #else
            #include <Dragon/hlsl/DefaultVSRelease.h> // for vs_default_bytecode
            #include <Dragon/hlsl/UnlitPSRelease.h> // for ps_unlit_bytecode
            #endif

            m_vs_default = D3D11Utils::LoadVertexShaderFromBytecode(m_device, vs_default_bytecode, Dragon_CountOf(vs_default_bytecode));
            m_ps_unlit = D3D11Utils::LoadPixelShaderFromBytecode(m_device, ps_unlit_bytecode, Dragon_CountOf(ps_unlit_bytecode));

            D3D11_INPUT_ELEMENT_DESC desc[]
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            m_input_layout = D3D11Utils::CreateInputLayout(m_device, desc, Dragon_CountOf(desc), vs_default_bytecode, Dragon_CountOf(vs_default_bytecode));
        }


        // NOTE: default rasterizer state
        {
            // NOTE: https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
            D3D11_RASTERIZER_DESC desc{};
            desc.FillMode = D3D11_FILL_SOLID;
            desc.CullMode = D3D11_CULL_BACK;
            desc.FrontCounterClockwise = TRUE;
            desc.DepthBias = 0;
            desc.DepthBiasClamp = 0.0f;
            desc.SlopeScaledDepthBias = 0.0f;
            desc.DepthClipEnable = TRUE;
            desc.ScissorEnable = FALSE;
            // NOTE: we recommend that you always set MultisampleEnable to TRUE whenever you render on MSAA render targets
            desc.MultisampleEnable = FALSE;
            // NOTE: it only applies if doing line drawing and MultisampleEnable is FALSE
            desc.AntialiasedLineEnable = FALSE;

            Dragon_CheckHR(m_device->CreateRasterizerState(&desc, m_rasterizer_state_default.ReleaseAndGetAddressOf()));
        }

        // NOTE: default depth stencil state
        {
            D3D11_DEPTH_STENCIL_DESC desc{};
            desc.DepthEnable = TRUE;
            desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc = D3D11_COMPARISON_LESS;
            //desc.StencilEnable = ;
            //desc.StencilReadMask = ;
            //desc.StencilWriteMask = ;
            //desc.FrontFace = ;
            //desc.BackFace = ;

            Dragon_CheckHR(m_device->CreateDepthStencilState(&desc, m_depth_stencil_state_default.ReleaseAndGetAddressOf()));
        }

        // NOTE: default sampler state
        {
            D3D11_SAMPLER_DESC desc{};
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            desc.MipLODBias = 0.0f;
            desc.MaxAnisotropy = 0;
            desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            desc.BorderColor[0] = 1.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[1] = 0.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[2] = 1.0f; // NOTE: obnoxious pink border color
            desc.BorderColor[3] = 1.0f; // NOTE: obnoxious pink border color
            desc.MinLOD = 0.0f;
            desc.MaxLOD = D3D11_FLOAT32_MAX;

            Dragon_CheckHR(m_device->CreateSamplerState(&desc, m_sampler_state_default.ReleaseAndGetAddressOf()));
        }

        // TODO: camera constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBCamera);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_camera.ReleaseAndGetAddressOf()));
            m_constant_buffers[0] = m_cb_camera.Get();
        }

        // NOTE: object constants
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(CBObject);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            Dragon_CheckHR(m_device->CreateBuffer(&desc, nullptr, m_cb_object.ReleaseAndGetAddressOf()));
            m_constant_buffers[1] = m_cb_object.Get();
        }
    }
}
