#include <Dragon/pch.h>
#include <Dragon/MeshRef.h>

namespace Dragon
{
    MeshRef::MeshRef(size_t* ref_count, Mesh* mesh)
        : m_ref_count{ ref_count }
        , m_mesh{ mesh }
    {
    }
    MeshRef::MeshRef() : MeshRef{ nullptr, nullptr }
    {
    }
    MeshRef::~MeshRef()
    {
        (*m_ref_count)--;
    }
    MeshRef::MeshRef(const MeshRef& other)
        : m_ref_count{}
        , m_mesh{}
    {
        *this = other;
    }
    MeshRef& MeshRef::operator=(const MeshRef& other)
    {
        if (m_ref_count) (*m_ref_count)--;

        m_ref_count = other.m_ref_count;
        m_mesh = other.m_mesh;

        if (m_ref_count) (*m_ref_count)++;

        return *this;
    }
}
