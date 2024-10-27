#pragma once

#include <Dragon/Mesh.h>

namespace Dragon
{
    class MeshRef
    {
    public:
        MeshRef(size_t* ref_count, Mesh* mesh);
        MeshRef();
        ~MeshRef();
        MeshRef(const MeshRef& other);
        MeshRef(MeshRef&& other) noexcept = delete;
        MeshRef& operator=(const MeshRef& other);
        MeshRef& operator=(MeshRef&& other) noexcept = delete;
    public:
        Mesh* Get() { return m_mesh; }
    private:
        size_t* m_ref_count;
        Mesh* m_mesh;
    };
}
