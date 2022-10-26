#pragma once

#include <d3d12.h>
#include "ComPtr.h"

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, size_t size, size_t stride, const void* pInitData = nullptr);

    // 初期化処理
    template<typename T>
    bool Init(ID3D12Device* pDevice,size_t size,const T* pInitData = nullptr)
    {
        return Init(pDevice, size, sizeof(T), pInitData);
    }

    // 終了処理
    void Term();

    // メモリマッピング
    void* Map();

    // メモリマッピングを解除
    void Unmap();

    // メモリマッピング
    template<typename T>
    T* Map() const
    {
        return reinterpret_cast<T*>(Map());
    }

    // 頂点バッファビューを取得
    D3D12_VERTEX_BUFFER_VIEW GetView() const;

private:
    ComPtr<ID3D12Resource>      m_pVB;      // 頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW    m_View;     // 頂点バッファビュー

    VertexBuffer(const VertexBuffer&) = delete;
    void operator = (const VertexBuffer&) = delete;
};