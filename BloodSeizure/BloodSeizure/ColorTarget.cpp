#include "ColorTarget.h"
#include "DescriptorPool.h"

// �R���X�g���N�^
ColorTarget::ColorTarget()
    : m_pTarget(nullptr)
    , m_pHandleRTV(nullptr)
    , m_pPoolRTV(nullptr)
{
}

// �f�X�g���N�^
ColorTarget::~ColorTarget()
{
    Term();
}

// ����������
bool ColorTarget::Init(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t width, uint32_t height, DXGI_FORMAT format)
{
    // �����`�F�b�N
    if (pDevice == nullptr || pPoolRTV == nullptr || width == 0 || height == 0)
    {
        return false;
    }

    assert(m_pHandleRTV == nullptr);
    assert(m_pPoolRTV == nullptr);

    m_pHandleRTV = m_pPoolRTV->AllocHandle();
    if (m_pHandleRTV == nullptr)
    {
        return false;
    }

    // �v���p�e�B�ݒ�
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_DEFAULT;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;


    return false;
}

// �o�b�N�o�b�t�@���珉�������J�n����
bool ColorTarget::InitFromBackBuffer(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t index, IDXGISwapChain* pSwapChain)
{
    return false;
}

// �������
void ColorTarget::Term()
{
}

// �f�B�X�N���v�^�n���h���iRTV�p�j���擾
DescriptorHandle* ColorTarget::GetHandleRTV() const
{
    return nullptr;
}

// ���\�[�X���擾
ID3D12Resource* ColorTarget::GetResource() const
{
    return nullptr;
}

// ���\�[�X�̍\���̂��擾
D3D12_RESOURCE_DESC ColorTarget::GetDesc() const
{
    return D3D12_RESOURCE_DESC();
}

// �r���[�\���̂��擾
D3D12_RENDER_TARGET_VIEW_DESC ColorTarget::GetViewDesc() const
{
    return D3D12_RENDER_TARGET_VIEW_DESC();
}
