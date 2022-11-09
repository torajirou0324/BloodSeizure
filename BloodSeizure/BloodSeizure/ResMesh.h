#pragma once
//#define NOMINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

struct ResMaterial
{
    DirectX::XMFLOAT3   Diffuse;        // �g�U���ː���
    DirectX::XMFLOAT3   Specular;       // ���ʔ��ː���
    float               Alpha;          // ���ߔ��ː���
    float               Shininess;      // ���ʔ��ˋ��x
    std::wstring        DiffuseMap;     // �f�B�t���[�Y�}�b�v�t�@�C���p�X
    std::wstring        SpecularMap;    // �X�y�L�����[�}�b�v�t�@�C���p�X
    std::wstring        ShininessMap;   // �V���C�l�X�}�b�v�t�@�C���p�X
    std::wstring        NormalMap;      // �@���}�b�v�t�@�C���p�X
};

class MeshVertex
{
public:
    DirectX::XMFLOAT3   Position;
    DirectX::XMFLOAT3   Normal;
    DirectX::XMFLOAT2   TexCoord;
    DirectX::XMFLOAT3   Tangent;

    MeshVertex() = default;

    MeshVertex(
        DirectX::XMFLOAT3 const& position,
        DirectX::XMFLOAT3 const& normal,
        DirectX::XMFLOAT2 const& texcoord,
        DirectX::XMFLOAT3 const& tangent)
        : Position(position)
        , Normal(normal)
        , TexCoord(texcoord)
        , Tangent(tangent)
    { /* DO_NOTHING */
    }

    static const D3D12_INPUT_LAYOUT_DESC InputLayout;

private:
    static const int InputElementCount = 4;
    static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

struct ResMesh
{
    std::vector<MeshVertex> Vertices;       // ���_�f�[�^
    std::vector<uint32_t>   Indices;        // ���_�C���f�b�N�X
    uint32_t                MaterialId;     // �}�e���A���ԍ�
};

bool LoadMesh(const wchar_t* filename, std::vector<ResMesh>& meshs, std::vector<ResMaterial>& materials);