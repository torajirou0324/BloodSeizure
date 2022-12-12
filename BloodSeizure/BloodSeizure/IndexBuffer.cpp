#include "IndexBuffer.h"

// �R���X�g���N�^
IndexBuffer::IndexBuffer()
{
}


// �f�X�g���N�^
IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Init(int size, int stride)
{
    int sizeInBytes = 0;
    if (stride == 2)
    {
        sizeInBytes == size * 2;
    }
    {
        sizeInBytes = size;
    }

    auto device = g_graphicsEngine->GetD3DDevice();

    // �q�[�v�v���p�e�B
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_UPLOAD;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = UINT64(size);
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // ���\�[�X�̐���
    auto hr = device->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_pIndexBuffer)
    );

    // �C���f�b�N�X�o�b�t�@�[�r���[���쐬
    m_indexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();

    // �X�g���C�h�͂S�o�C�g�Œ�
    m_strideInBytes = 4;
    m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_indexBufferView.SizeInBytes = UINT(sizeInBytes);
    
    m_count = sizeInBytes / m_strideInBytes;
}

void IndexBuffer::Copy(uint16_t* srcIndices)
{
    uint32_t* pData;
    m_pIndexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
    for (int i = 0; i < m_count; i++)
    {
        pData[i] = srcIndices[i];
    }
    m_pIndexBuffer->Unmap(0, nullptr);
}

void IndexBuffer::Copy(uint32_t* srcIndices)
{
    uint32_t* pData;
    m_pIndexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
    for (int i = 0; i < m_count; i++)
    {
        pData[i] = srcIndices[i];
    }
    m_pIndexBuffer->Unmap(0, nullptr);
}

// �C���f�b�N�X�o�b�t�@�[�r���[�̎擾
D3D12_INDEX_BUFFER_VIEW IndexBuffer::GetView() const
{
    return m_indexBufferView;
}

int IndexBuffer::GetCount() const
{
    return m_count;
}

UINT IndexBuffer::GetStrideInBytes() const
{
    return m_strideInBytes;
}

ID3D12Resource* IndexBuffer::GetID3D12ResourceAddress() const
{
    return m_pIndexBuffer;
}
