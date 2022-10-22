#pragma once

#include <d3d12.h>
#include <atomic>
#include "ComPtr.h"
#include "Pool.h"

class DescriptorHandle
{
public:
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;  // CPUディスクリプタハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;  // GPUディスクリプタハンドル

    bool HasCPU() const
    {
        return HandleCPU.ptr != 0;
    }

    bool HasGPU() const
    {
        return HandleGPU.ptr != 0;
    }
};

class DescriptorPool
{
public:
    // 生成処理
    static bool Create(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* pDesc, DescriptorPool** ppPool);

    // 参照カウントを増やす
    void AddRef();

    // 解放処理
    void Release();

    // 参照カウントを取得
    uint32_t GetCount() const;

    // ディスクリプタハンドルを割り当てる
    DescriptorHandle* AllocHandle();

    // ディスクリプタハンドルを解放する
    void FreeHandle(DescriptorHandle*& pHandle);

    // 利用可能なハンドルを返す
    uint32_t GetAvailableHandleCount() const;

    // 割り当て済みのハンドルを取得
    uint32_t GetAllocatedHandleCount() const;

    // ハンドル数を取得
    uint32_t GetHandleCount() const;

    // ディスクリプタヒープを取得
    ID3D12DescriptorHeap* const GetHeap() const;

private:
    std::atomic<uint32_t>           m_RefCount;         // 参照カウント
    Pool<DescriptorHandle>          m_Pool;             // ディスクリプタハンドルプール
    ComPtr<ID3D12DescriptorHeap>    m_pHeap;            // ディスクリプタヒープ
    uint32_t                        m_DescriptorSize;   // ディスクリプタサイズ

    DescriptorPool();   // コンストラクタ
    ~DescriptorPool();  // デストラクタ

    DescriptorPool(const DescriptorPool&) = delete;
    void operator = (const DescriptorPool&) = delete;
};