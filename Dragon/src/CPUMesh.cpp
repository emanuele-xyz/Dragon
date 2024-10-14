#include <Dragon/pch.h>
#include <Dragon/CPUMesh.h>
#include <Dragon/Commons.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dragon
{
    CPUMesh CPUMesh::LoadFromFile(const std::string& path)
    {
        Assimp::Importer importer{};
        const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate) };
        Dragon_Check(scene);
        Dragon_Check(scene->mNumMeshes == 1);
        aiMesh* mesh{ scene->mMeshes[0] };
        Dragon_Check(mesh->mNumVertices > 0);
        Dragon_Check(mesh->mNumFaces > 0);
        Dragon_Check(mesh->mTextureCoords[0]);

        CPUMesh cpu_mesh{ mesh->mNumVertices, mesh->mNumFaces * 3 };
        auto positions{ cpu_mesh.GetPositions() };
        auto normals{ cpu_mesh.GetNormals() };
        auto uvs{ cpu_mesh.GetUVs() };
        auto indices{ cpu_mesh.GetRawIndices() };

        // NOTE: read vertex data
        for (unsigned int i{}; i < mesh->mNumVertices; i++)
        {
            positions[i] = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
            normals[i] = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            uvs[i] = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }

        // NOTE: read index data
        for (unsigned int i{}; i < mesh->mNumFaces; i++)
        {
            aiFace face{ mesh->mFaces[i] };
            // NOTE: each face must be a triangle
            Dragon_Check(face.mNumIndices == 3);

            indices[i * 3 + 0] = face.mIndices[0];
            indices[i * 3 + 1] = face.mIndices[1];
            indices[i * 3 + 2] = face.mIndices[2];
        }

        return cpu_mesh;
    }

    CPUMesh::CPUMesh(unsigned int vertex_count, unsigned int index_count)
        : m_vertex_count{ vertex_count }
        , m_index_count{ index_count }
        , m_vertex_buffers{}
        , m_indices{}
    {
        for (int i{}; i < static_cast<int>(Mesh::VertexBufferIdx::Count); i++)
        {
            m_vertex_buffers[i] = std::make_unique<float[]>(m_vertex_count * Mesh::GetFloatsPerVertex(i));
        }
        m_indices = std::make_unique<uint32_t[]>(m_index_count);
    }
}
