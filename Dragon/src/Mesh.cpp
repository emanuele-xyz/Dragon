#include <Dragon/pch.h>
#include <Dragon/Mesh.h>
#include <Dragon/Commons.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dragon
{
    Mesh::Mesh(ID3D11Device* device, const std::filesystem::path& path)
        : m_vertex_count{}
        , m_index_count{}
        , m_positions{}
        , m_normals{}
        , m_uvs{}
        , m_indices{}
        , m_vertex_buffers{}
        , m_strides{}
        , m_offsets{}
    {
        Assimp::Importer importer{};
        const aiScene* scene{ importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs) };
        Dragon_Check(scene);
        Dragon_Check(scene->mNumMeshes == 1);
        aiMesh* mesh{ scene->mMeshes[0] };
        Dragon_Check(mesh->mNumVertices > 0);
        Dragon_Check(mesh->mNumFaces > 0);
        Dragon_Check(mesh->mTextureCoords[0]);

        m_vertex_count = mesh->mNumVertices;
        m_index_count = mesh->mNumFaces * 3;

        auto positions{ std::make_unique<float[]>(m_vertex_count * DRAGON_MESH_FLOATS_PER_POSITION) };
        auto normals{ std::make_unique<float[]>(m_vertex_count * DRAGON_MESH_FLOATS_PER_NORMAL) };
        auto uvs{ std::make_unique<float[]>(m_vertex_count * DRAGON_MESH_FLOATS_PER_UV) };
        auto indices{ std::make_unique<uint32_t[]>(m_index_count) };

        // NOTE: read vertex data
        for (unsigned int i{}; i < m_vertex_count; i++)
        {
            positions[i * DRAGON_MESH_FLOATS_PER_POSITION + 0] = mesh->mVertices[i].x;
            positions[i * DRAGON_MESH_FLOATS_PER_POSITION + 1] = mesh->mVertices[i].y;
            positions[i * DRAGON_MESH_FLOATS_PER_POSITION + 2] = mesh->mVertices[i].z;
            normals[i * DRAGON_MESH_FLOATS_PER_NORMAL + 0] = mesh->mNormals[i].x;
            normals[i * DRAGON_MESH_FLOATS_PER_NORMAL + 1] = mesh->mNormals[i].y;
            normals[i * DRAGON_MESH_FLOATS_PER_NORMAL + 2] = mesh->mNormals[i].z;
            uvs[i * DRAGON_MESH_FLOATS_PER_UV + 0] = mesh->mTextureCoords[0][i].x;
            uvs[i * DRAGON_MESH_FLOATS_PER_UV + 1] = mesh->mTextureCoords[0][i].y;
        }

        // NOTE: read index data
        for (unsigned int i{}; i < mesh->mNumFaces; i++)
        {
            aiFace face{ mesh->mFaces[i] };
            Dragon_Check(face.mNumIndices == 3); // each face must be a triangle

            indices[i * 3 + 0] = face.mIndices[0];
            indices[i * 3 + 1] = face.mIndices[1];
            indices[i * 3 + 2] = face.mIndices[2];
        }

        // NOTE: upload vertices
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = m_vertex_count * DRAGON_MESH_FLOATS_PER_POSITION * sizeof(float);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = positions.get();
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            m_positions = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
            m_vertex_buffers[0] = m_positions.Get();
            m_strides[0] = DRAGON_MESH_FLOATS_PER_POSITION * sizeof(float);
            m_offsets[0] = 0;
        }

        // NOTE: upload normals
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = m_vertex_count * DRAGON_MESH_FLOATS_PER_NORMAL * sizeof(float);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = normals.get();
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            m_normals = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
            m_vertex_buffers[1] = m_normals.Get();
            m_strides[1] = DRAGON_MESH_FLOATS_PER_NORMAL * sizeof(float);
            m_offsets[1] = 0;
        }

        // NOTE: upload uvs
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = m_vertex_count * DRAGON_MESH_FLOATS_PER_UV * sizeof(float);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = uvs.get();
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            m_uvs = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
            m_vertex_buffers[2] = m_uvs.Get();
            m_strides[2] = DRAGON_MESH_FLOATS_PER_UV * sizeof(float);
            m_offsets[2] = 0;
        }

        // NOTE: index buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = m_index_count * sizeof(uint32_t);
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subres_data{};
            subres_data.pSysMem = indices.get();
            subres_data.SysMemPitch = 0;
            subres_data.SysMemSlicePitch = 0;

            m_indices = D3D11Utils::CreateBuffer(device, &desc, &subres_data);
        }
    }
}