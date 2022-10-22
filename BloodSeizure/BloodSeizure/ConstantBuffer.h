#pragma once

#include <d3d12.h>
#include <vector>
#include "ComPtr.h"

class DescriptorHandle;
class DescriptorPool;

class ConstantBuffer
{
public:
    ConstantBuffer();
    ~ConstantBuffer();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, size_t size);

    // 解放処理
    void Term();

    // GPU仮想アドレスを取得する
    D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;

    // CPUディスクリプタハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU() const;

    // GPUディスクリプタハンドルを取得
    D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU() const;

    // メモリマッピング済みポインタを取得
    void* GetPtr() const;

    // メモリマッピング済みポインタを取得
    template<typename T>
    T* GetPtr()
    {
        return reinterpret_cast<T*>(GetPtr());
    }

private:
    ComPtr<ID3D12Resource>          m_pCB;          // 定数バッファ
    DescriptorHandle*               m_pHandle;      // ディスクリプタハンドル
    DescriptorPool*                 m_pPool;        // ディスクリプタプール
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_Desc;         // 定数バッファビュー構造体
    void*                           m_pMappedPtr;   // マップ済みポインタ

    ConstantBuffer(const ConstantBuffer&) = delete;
    void operator = (const ConstantBuffer&) = delete;
};