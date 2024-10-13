#pragma once

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
	class ImGuiHandle
	{
	public:
		ImGuiHandle(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
		~ImGuiHandle();
		ImGuiHandle(const ImGuiHandle&) = delete;
		ImGuiHandle(ImGuiHandle&&) noexcept = delete;
		ImGuiHandle& operator=(const ImGuiHandle&) = delete;
		ImGuiHandle& operator=(ImGuiHandle&&) noexcept = delete;
	public:
		void NewFrame();
		void Render();
	};
}
