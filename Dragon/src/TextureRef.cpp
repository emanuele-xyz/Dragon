#include <Dragon/pch.h>
#include <Dragon/TextureRef.h>

namespace Dragon
{
    TextureRef::TextureRef(size_t* ref_count, Texture* texture)
        : m_ref_count{ ref_count }
        , m_texture{ texture }
    {
    }
    TextureRef::TextureRef() : TextureRef{ nullptr, nullptr }
    {
    }
    TextureRef::~TextureRef()
    {
        (*m_ref_count)--;
    }
    TextureRef::TextureRef(const TextureRef& other)
        : m_ref_count{}
        , m_texture{}
    {
        *this = other;
    }
    TextureRef& TextureRef::operator=(const TextureRef& other)
    {
        if (m_ref_count) (*m_ref_count)--;

        m_ref_count = other.m_ref_count;
        m_texture = other.m_texture;

        if (m_ref_count) (*m_ref_count)++;

        return *this;
    }
}