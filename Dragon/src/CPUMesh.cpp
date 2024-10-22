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
        const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };
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

    CPUMesh CPUMesh::Cube()
    {
        CPUMesh cpu_mesh{ 36, 36 };

        auto positions{ cpu_mesh.GetPositions() };
        positions[0] = { -1.0f, +1.0f, -1.0f };
        positions[1] = { +1.0f, +1.0f, +1.0f };
        positions[2] = { +1.0f, +1.0f, -1.0f };
        positions[3] = { +1.0f, +1.0f, +1.0f };
        positions[4] = { -1.0f, -1.0f, +1.0f };
        positions[5] = { +1.0f, -1.0f, +1.0f };
        positions[6] = { -1.0f, +1.0f, +1.0f };
        positions[7] = { -1.0f, -1.0f, -1.0f };
        positions[8] = { -1.0f, -1.0f, +1.0f };
        positions[9] = { +1.0f, -1.0f, -1.0f };
        positions[10] = { -1.0f, -1.0f, +1.0f };
        positions[11] = { -1.0f, -1.0f, -1.0f };
        positions[12] = { +1.0f, +1.0f, -1.0f };
        positions[13] = { +1.0f, -1.0f, +1.0f };
        positions[14] = { +1.0f, -1.0f, -1.0f };
        positions[15] = { -1.0f, +1.0f, -1.0f };
        positions[16] = { +1.0f, -1.0f, -1.0f };
        positions[17] = { -1.0f, -1.0f, -1.0f };
        positions[18] = { -1.0f, +1.0f, -1.0f };
        positions[19] = { -1.0f, +1.0f, +1.0f };
        positions[20] = { +1.0f, +1.0f, +1.0f };
        positions[21] = { +1.0f, +1.0f, +1.0f };
        positions[22] = { -1.0f, +1.0f, +1.0f };
        positions[23] = { -1.0f, -1.0f, +1.0f };
        positions[24] = { -1.0f, +1.0f, +1.0f };
        positions[25] = { -1.0f, +1.0f, -1.0f };
        positions[26] = { -1.0f, -1.0f, -1.0f };
        positions[27] = { +1.0f, -1.0f, -1.0f };
        positions[28] = { +1.0f, -1.0f, +1.0f };
        positions[29] = { -1.0f, -1.0f, +1.0f };
        positions[30] = { +1.0f, +1.0f, -1.0f };
        positions[31] = { +1.0f, +1.0f, +1.0f };
        positions[32] = { +1.0f, -1.0f, +1.0f };
        positions[33] = { -1.0f, +1.0f, -1.0f };
        positions[34] = { +1.0f, +1.0f, -1.0f };
        positions[35] = { +1.0f, -1.0f, -1.0f };

        auto normals{ cpu_mesh.GetNormals() };
        normals[0] = { -0.0f, +1.0f, -0.0f };
        normals[1] = { -0.0f, +1.0f, -0.0f };
        normals[2] = { -0.0f, +1.0f, -0.0f };
        normals[3] = { -0.0f, -0.0f, +1.0f };
        normals[4] = { -0.0f, -0.0f, +1.0f };
        normals[5] = { -0.0f, -0.0f, +1.0f };
        normals[6] = { -1.0f, -0.0f, -0.0f };
        normals[7] = { -1.0f, -0.0f, -0.0f };
        normals[8] = { -1.0f, -0.0f, -0.0f };
        normals[9] = { -0.0f, -1.0f, -0.0f };
        normals[10] = { -0.0f, -1.0f, -0.0f };
        normals[11] = { -0.0f, -1.0f, -0.0f };
        normals[12] = { +1.0f, -0.0f, -0.0f };
        normals[13] = { +1.0f, -0.0f, -0.0f };
        normals[14] = { +1.0f, -0.0f, -0.0f };
        normals[15] = { -0.0f, -0.0f, -1.0f };
        normals[16] = { -0.0f, -0.0f, -1.0f };
        normals[17] = { -0.0f, -0.0f, -1.0f };
        normals[18] = { -0.0f, +1.0f, -0.0f };
        normals[19] = { -0.0f, +1.0f, -0.0f };
        normals[20] = { -0.0f, +1.0f, -0.0f };
        normals[21] = { -0.0f, -0.0f, +1.0f };
        normals[22] = { -0.0f, -0.0f, +1.0f };
        normals[23] = { -0.0f, -0.0f, +1.0f };
        normals[24] = { -1.0f, -0.0f, -0.0f };
        normals[25] = { -1.0f, -0.0f, -0.0f };
        normals[26] = { -1.0f, -0.0f, -0.0f };
        normals[27] = { -0.0f, -1.0f, -0.0f };
        normals[28] = { -0.0f, -1.0f, -0.0f };
        normals[29] = { -0.0f, -1.0f, -0.0f };
        normals[30] = { +1.0f, -0.0f, -0.0f };
        normals[31] = { +1.0f, -0.0f, -0.0f };
        normals[32] = { +1.0f, -0.0f, -0.0f };
        normals[33] = { -0.0f, -0.0f, -1.0f };
        normals[34] = { -0.0f, -0.0f, -1.0f };
        normals[35] = { -0.0f, -0.0f, -1.0f };

        auto uvs{ cpu_mesh.GetUVs() };
        uvs[0] = { 1.0f, 1.0f };
        uvs[1] = { 0.0f, 0.0f };
        uvs[2] = { 0.0f, 1.0f };
        uvs[3] = { 1.0f, 1.0f };
        uvs[4] = { 0.0f, 0.0f };
        uvs[5] = { 0.0f, 1.0f };
        uvs[6] = { 1.0f, 1.0f };
        uvs[7] = { 0.0f, 0.0f };
        uvs[8] = { 0.0f, 1.0f };
        uvs[9] = { 1.0f, 1.0f };
        uvs[10] = { 0.0f, 0.0f };
        uvs[11] = { 0.0f, 1.0f };
        uvs[12] = { 1.0f, 1.0f };
        uvs[13] = { 0.0f, 0.0f };
        uvs[14] = { 0.0f, 1.0f };
        uvs[15] = { 1.0f, 1.0f };
        uvs[16] = { 0.0f, 0.0f };
        uvs[17] = { 0.0f, 1.0f };
        uvs[18] = { 1.0f, 1.0f };
        uvs[19] = { 1.0f, 0.0f };
        uvs[20] = { 0.0f, 0.0f };
        uvs[21] = { 1.0f, 1.0f };
        uvs[22] = { 1.0f, 0.0f };
        uvs[23] = { 0.0f, 0.0f };
        uvs[24] = { 1.0f, 1.0f };
        uvs[25] = { 1.0f, 0.0f };
        uvs[26] = { 0.0f, 0.0f };
        uvs[27] = { 1.0f, 1.0f };
        uvs[28] = { 1.0f, 0.0f };
        uvs[29] = { 0.0f, 0.0f };
        uvs[30] = { 1.0f, 1.0f };
        uvs[31] = { 1.0f, 0.0f };
        uvs[32] = { 0.0f, 0.0f };
        uvs[33] = { 1.0f, 1.0f };
        uvs[34] = { 1.0f, 0.0f };
        uvs[35] = { 0.0f, 0.0f };

        auto indices{ cpu_mesh.GetIndices() };
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 3;
        indices[4] = 4;
        indices[5] = 5;
        indices[6] = 6;
        indices[7] = 7;
        indices[8] = 8;
        indices[9] = 9;
        indices[10] = 10;
        indices[11] = 11;
        indices[12] = 12;
        indices[13] = 13;
        indices[14] = 14;
        indices[15] = 15;
        indices[16] = 16;
        indices[17] = 17;
        indices[18] = 18;
        indices[19] = 19;
        indices[20] = 20;
        indices[21] = 21;
        indices[22] = 22;
        indices[23] = 23;
        indices[24] = 24;
        indices[25] = 25;
        indices[26] = 26;
        indices[27] = 27;
        indices[28] = 28;
        indices[29] = 29;
        indices[30] = 30;
        indices[31] = 31;
        indices[32] = 32;
        indices[33] = 33;
        indices[34] = 34;
        indices[35] = 35;

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
