#include "ResMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <codecvt>
#include <cassert>

// UTF-8������֕ϊ�
std::string ToUTF8(const std::wstring& value)
{
    auto length = WideCharToMultiByte(
        CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
    auto buffer = new char[length];

    WideCharToMultiByte(
        CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

    std::string result(buffer);
    delete[] buffer;
    buffer = nullptr;

    return result;
}

// std::wstring�^�ɕϊ�
std::wstring Convert(const aiString& path)
{
    wchar_t temp[256] = {};
    size_t size;
    mbstowcs_s(&size, temp, path.C_Str(), 256);
    return std::wstring(temp);
}

class MeshLoader
{
public:
    MeshLoader();
    ~MeshLoader();

    bool Load(const wchar_t* filename, std::vector<ResMesh>& meshes, std::vector<ResMaterial>& materials);

private:
    const aiScene* m_pScene = nullptr;  // �V�[���f�[�^

    void ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh);
    void ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial);
};

// �R���X�g���N�^
MeshLoader::MeshLoader()
    : m_pScene(nullptr)
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::Load(const wchar_t* filename, std::vector<ResMesh>& meshes, std::vector<ResMaterial>& materials)
{
    if (filename == nullptr)
    {
        return false;
    }

    // wchar_t���� char�^�iUTF-8�j�ɕϊ�
    auto path = ToUTF8(filename);

    Assimp::Importer importer;
    unsigned int flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;

    // �t�@�C����ǂݍ���
    m_pScene = importer.ReadFile(path, flag);

    // �`�F�b�N
    if (m_pScene == nullptr)
    {
        return false;
    }

    // ���b�V���̃��������m��
    meshes.clear();
    meshes.resize(m_pScene->mNumMeshes);

    // ���b�V���f�[�^��ϊ�
    for (size_t i = 0; i < meshes.size(); ++i)
    {
        const auto pMesh = m_pScene->mMeshes[i];
        ParseMesh(meshes[i], pMesh);
    }

    // �}�e���A���̃��������m��
    materials.clear();
    materials.resize(m_pScene->mNumMaterials);

    // �}�e���A���f�[�^��ϊ�
    for (size_t i = 0; i < materials.size(); ++i)
    {
        const auto pMaterial = m_pScene->mMaterials[i];
        ParseMaterial(materials[i], pMaterial);
    }

    // �s�v�ɂȂ����̂ŃN���A
    importer.FreeScene();
    m_pScene = nullptr;

    // ����I��
    return true;
}

// ���b�V���f�[�^�����
void MeshLoader::ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh)
{
    // �}�e���A���ԍ���ݒ�
    dstMesh.MaterialId = pSrcMesh->mMaterialIndex;

    aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // ���_�f�[�^�̃��������m��
    dstMesh.Vertices.resize(pSrcMesh->mNumVertices);

    for (auto i = 0u; i < pSrcMesh->mNumVertices; ++i)
    {
        auto pPosition = &(pSrcMesh->mVertices[i]);
        auto pNormal = &(pSrcMesh->mNormals[i]);
        auto pTexCoord = (pSrcMesh->mTextureCoords[0]) ? &(pSrcMesh->mTextureCoords[0][i]) : &zero3D;
        auto pTangent = (pSrcMesh->HasTangentsAndBitangents()) ? &(pSrcMesh->mTangents[i]) : &zero3D;

        dstMesh.Vertices[i] = MeshVertex(
            DirectX::XMFLOAT3(pPosition->x, pPosition->y, pPosition->z),
            DirectX::XMFLOAT3()
        )
    }
}

void MeshLoader::ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial)
{
}

bool LoadMesh(const wchar_t* filename, std::vector<ResMesh>& meshs, std::vector<ResMaterial>& materials)
{
    return false;
}