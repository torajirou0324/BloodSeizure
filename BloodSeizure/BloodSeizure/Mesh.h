#pragma once

#include "ResMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, const ResMesh& resource);

    // 終了処理
    void Term();

    // 描画処理
    void Draw(ID3D12GraphicsCommandList* pCmdList);

    // マテリアルIDを取得します
    uint32_t GetMaterialId() const;

private:
    VertexBuffer    m_VB;               // 頂点バッファ
    IndexBuffer     m_IB;               // インデックスバッファ
    uint32_t        m_MaterialId;       // マテリアルID
    uint32_t        m_IndexCount;       // インデックス数

    Mesh            (const Mesh&) = delete;
    void operator = (const Mesh&) = delete;
};