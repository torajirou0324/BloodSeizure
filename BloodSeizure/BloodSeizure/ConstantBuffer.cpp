#include "ConstantBuffer.h"
#include <vector>

// コンストラクタ
ConstantBuffer::ConstantBuffer()
{
}

// デストラクタ
ConstantBuffer::~ConstantBuffer()
{
    Term();
}

void ConstantBuffer::Init(int size, void* srcData)
{
    m_size = size;

    size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
    UINT64 sizeAligned = (size + (align - 1)) & ~(align - 1); // alignに切り上げる

    // ヒーププロパティ
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_UPLOAD;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    // リソースの設定
    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeAligned;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // リソースを生成
    int bufferNum = 0;
    auto device = g_graphicsEngine->GetD3DDevice();
    for (auto& cb : m_pConstantBuffer)
    {
        auto hr = device->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(cb.GetAddressOf())
        );

        {
            D3D12_RANGE rg = {};
            rg.Begin = 0;
            rg.End = 0;
            cb->Map(0, &rg, reinterpret_cast<void**>(&m_pConstantBufferCPU[bufferNum]));
        }
        if (srcData != nullptr)
        {
            memcpy(m_pConstantBufferCPU[bufferNum], srcData, m_size);
        }

        // メモリマッピング
        hr = cb->Map(0, nullptr, &m_pConstantBufferCPU[bufferNum]);

        bufferNum++;
    }
}

// 解放処理
void ConstantBuffer::Term()
{
    // メモリマッピングを解除し、定数バッファを解放

    // ビューを破壊

    // ディスクリプタプールを解放
    for (int i = 0; i < 2; i++)
    {
        m_pConstantBufferCPU[i] = nullptr;
    }

}

void ConstantBuffer::RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
{
    auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
    RegistConstantBufferView(descriptorHandle, backBufferIndex);
}

void ConstantBuffer::RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    //D3Dデバイスを取得。
    auto device = g_graphicsEngine->GetD3DDevice();
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
    desc.BufferLocation = m_pConstantBuffer[bufferNo]->GetGPUVirtualAddress();
    desc.SizeInBytes = m_alocSize;
    device->CreateConstantBufferView(&desc, descriptorHandle);
}

void ConstantBuffer::CopyToVRAM(void* data)
{
    auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
    memcpy(m_pConstantBufferCPU[backBufferIndex], data, m_size);
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGPUVirtualAddress()
{
    auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
    return m_pConstantBuffer[backBufferIndex]->GetGPUVirtualAddress();
}