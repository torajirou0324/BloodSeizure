#include "VertexBuffer.h"

// �R���X�g���N�^
VertexBuffer::VertexBuffer()
{
}

// �f�X�g���N�^
VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Init(void* size, int stride)
{
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

    // ���\�[�X�𐶐�
    auto hr = device->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_pVertexBuffer)
        );
    if (FAILED(hr))
    {
        return;
    }

    // �}�b�s���O����
    void* ptr = nullptr;
    m_pVertexBuffer->Map(0, nullptr, &ptr);

    // ���_�f�[�^���}�b�s���O��ɐݒ�
    memcpy(ptr, size, stride);

    // �}�b�s���O����
    m_pVertexBuffer->Unmap(0, nullptr);

    // ���_�o�b�t�@�r���[�̐ݒ�
    m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = UINT(stride);
    m_vertexBufferView.SizeInBytes = UINT(size);
}

//void VertexBuffer::Copy(void* srcVertices)
//{
//    uint8_t* pData = 0;
//    m_pVertexBuffer->Map(0, nullptr, (void**)pData);
//    memcpy(pData, srcVertices, m_vertexBufferView.SizeInBytes);
//    m_pVertexBuffer->Unmap(0, nullptr);
//}

ID3D12Resource* VertexBuffer::GetID3DResourceAddress() const
{
    return m_pVertexBuffer;
}

// ���_�o�b�t�@�r���[�̎擾
D3D12_VERTEX_BUFFER_VIEW VertexBuffer::GetView() const
{
    return m_vertexBufferView;
}
