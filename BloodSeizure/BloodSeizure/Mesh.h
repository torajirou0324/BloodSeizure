//-----------------------------------------------------------------------------
// File : Model.h
// Desc : Model Module.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////
// MeshVertex structure
///////////////////////////////////////////////////////////////////////////////
struct MeshVertex
{
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

///////////////////////////////////////////////////////////////////////////////
// Material structure
///////////////////////////////////////////////////////////////////////////////
struct Material
{
    DirectX::XMFLOAT3   Diffuse;        //!< �g�U���ː����ł�.
    DirectX::XMFLOAT3   Specular;       //!< ���ʔ��ː����ł�.
    float               Alpha;          //!< ���ߐ����ł�.
    float               Shininess;      //!< ���ʔ��ˋ��x�ł�.
    std::string         DiffuseMap;     //!< �e�N�X�`���t�@�C���p�X�ł�.
};

///////////////////////////////////////////////////////////////////////////////
// Mesh structure
///////////////////////////////////////////////////////////////////////////////
struct Mesh
{
    std::vector<MeshVertex>     Vertices;     //!< ���_�f�[�^�ł�.
    std::vector<uint32_t>       Indices;      //!< ���_�C���f�b�N�X�ł�.
    uint32_t                    MaterialId;   //!< �}�e���A���ԍ��ł�.
};

//-----------------------------------------------------------------------------
//! @brief      ���b�V�������[�h���܂�.
//!
//! @param[in]      filename        �t�@�C���p�X.
//! @param[out]     meshes          ���b�V���̊i�[��ł�.
//! @param[out]     materials       �}�e���A���̊i�[��ł�.
//! @retval true    ���[�h�ɐ���.
//! @retval false   ���[�h�Ɏ��s.
//-----------------------------------------------------------------------------
bool LoadMesh(
    const wchar_t* filename,
    std::vector<Mesh>& meshes,
    std::vector<Material>& materials);
