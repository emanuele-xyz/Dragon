#pragma once

#include <Dragon/Texture.h>

namespace Dragon
{
    class TextureRef
    {
    public:
        TextureRef(size_t* ref_count, Texture* texture);
        TextureRef();
        ~TextureRef();
        TextureRef(const TextureRef& other);
        TextureRef(TextureRef&&) noexcept = delete;
        TextureRef& operator=(const TextureRef& other);
        TextureRef& operator=(TextureRef&&) noexcept = delete;
    public:
        Texture* operator->() { return m_texture; }
    private:
        size_t* m_ref_count;
        Texture* m_texture;
    };
}
