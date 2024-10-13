#pragma once

#include <string>

#include <DirectXTex.h>

namespace Dragon
{
    // TODO: don't inherit from DirectX::ScratchImage. Just use a chunk of 8 bit integers with some format information attached
    class CPUTexture : public DirectX::ScratchImage
    {
    public:
        static CPUTexture LoadFromFile(const std::string& path);
    public:
        CPUTexture() = default;
        ~CPUTexture() = default;
        CPUTexture(const CPUTexture&) = delete;
        CPUTexture(CPUTexture&&) noexcept = default;
        CPUTexture& operator=(const CPUTexture&) = delete;
        CPUTexture& operator=(CPUTexture&&) noexcept = default;
    private:
    };
}
