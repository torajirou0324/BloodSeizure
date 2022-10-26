#include "IndexBuffer.h"

// �R���X�g���N�^
IndexBuffer::IndexBuffer()
    : m_pIB(nullptr)
{
    memset(&m_View, 0, sizeof(m_View));
}


// �f�X�g���N�^
IndexBuffer::~IndexBuffer()
{
    Term();
}

// ����������
bool IndexBuffer::Init(ID3D12Device* pDevice, size_t size, const uint32_t* pInitData)
{
    // �����`�F�b�N
    if (pDevice == nullptr || size == 0 || pInitData == nullptr)
    {
        return false;
    }

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
    auto hr = pDevice->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_pIB.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

    // �C���f�b�N�X�o�b�t�@�[�̐ݒ�
    m_View.BufferLocation = m_pIB->GetGPUVirtualAddress();
    m_View.Format = DXGI_FORMAT_R32_UINT;
    m_View.SizeInBytes = UINT(size);

    // �������f�[�^������΁A��������ł���
    if (pInitData != nullptr)
    {
        void* ptr = Map();
        if (ptr == nullptr)
        {
            return false;
        }

        memcpy(ptr, pInitData, size);
    }

    // ����I��
    return true;
}

// �������
void IndexBuffer::Term()
{
    m_pIB.Reset();
    memset(&m_View, 0, sizeof(m_View));
}

// �������}�b�s���O
uint32_t* IndexBuffer::Map()
{
    uint32_t* ptr;
    auto hr = m_pIB->Map(0, nullptr, reinterpret_cast<void**>(&ptr));
    if (FAILED(hr))
    {
        return nullptr;
    }
    return ptr;
}

// �������}�b�s���O����
void IndexBuffer::Unmap()
{
    m_pIB->Unmap(0, nullptr);
}

// �C���f�b�N�X�o�b�t�@�[�r���[�̎擾
D3D12_INDEX_BUFFER_VIEW IndexBuffer::GetView() const
{
    return m_View;
}
