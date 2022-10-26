#pragma once

#include <d3d12.h>
#include <cstdint>
#include "ComPtr.h"

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, size_t size, const uint32_t* pInitData = nullptr);

    // 解放処理
    void Term();

    // メモリマッピング
    uint32_t* Map();

    // メモリマッピング解除
    void Unmap();

    // インデックスバッファーを取得
    D3D12_INDEX_BUFFER_VIEW GetView() const;

private:
    ComPtr<ID3D12Resource>  m_pIB;      // インデックスバッファー
    D3D12_INDEX_BUFFER_VIEW m_View;     // インデックスバッファービュー

    IndexBuffer(const IndexBuffer&) = delete;
    void operator = (const IndexBuffer&) = delete;
};