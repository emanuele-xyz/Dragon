#pragma once

#include <Dragon/D3D11Utils.h>

namespace Dragon
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;
	private:
	};
}
