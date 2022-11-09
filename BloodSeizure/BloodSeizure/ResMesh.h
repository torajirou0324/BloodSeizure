#pragma once
//#define NOMINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

struct ResMaterial
{
    DirectX::XMFLOAT3   Diffuse;        // 拡散反射成分
    DirectX::XMFLOAT3   Specular;       // 鏡面反射成分
    float               Alpha;          // 透過反射成分
    float               Shininess;      // 鏡面反射強度
    std::wstring        DiffuseMap;     // ディフューズマップファイルパス
    std::wstring        SpecularMap;    // スペキュラーマップファイルパス
    std::wstring        ShininessMap;   // シャイネスマップファイルパス
    std::wstring        NormalMap;      // 法線マップファイルパス
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
    std::vector<MeshVertex> Vertices;       // 頂点データ
    std::vector<uint32_t>   Indices;        // 頂点インデックス
    uint32_t                MaterialId;     // マテリアル番号
};

bool LoadMesh(const wchar_t* filename, std::vector<ResMesh>& meshs, std::vector<ResMaterial>& materials);