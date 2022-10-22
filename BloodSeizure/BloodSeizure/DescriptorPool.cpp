#include "DescriptorPool.h"

// ��������
bool DescriptorPool::Create(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* pDesc, DescriptorPool** ppPool)
{
    // �����`�F�b�N
    if (pDevice == nullptr || pDesc == nullptr || ppPool == nullptr)
    {
        return false;
    }

    // �C���X�^���X�𐶐����܂�
    auto instance = new (std::nothrow) DescriptorPool();
    if (instance == nullptr)
    {
        return false;
    }

    // �f�B�X�N���v�^�q�[�v�𐶐�
    auto hr = pDevice->CreateDescriptorHeap(
        pDesc,
        IID_PPV_ARGS(instance->m_pHeap.GetAddressOf())
    );

    // ���s���������������s���I��
    if (FAILED(hr))
    {
        instance->Release();
        return false;
    }

    // �v�[����������
    if (!instance->m_Pool.Init(pDesc->NumDescriptors))
    {
        instance->Release();
        return false;
    }

    // �f�B�X�N���v�^�̉��Z�T�C�Y���擾
    instance->m_DescriptorSize =
        pDevice->GetDescriptorHandleIncrementSize(pDesc->Type);

    // �C���X�^���X��ݒ�
    *ppPool = instance;

    // ����I��
    return true;

    return false;
}

// �Q�ƃJ�E���g�𑝂₷
void DescriptorPool::AddRef()
{
    m_RefCount++;
}

// �������
void DescriptorPool::Release()
{
    m_RefCount--;
    if (m_RefCount == 0)
    {
        delete this;
    }
}

// �Q�ƃJ�E���g���擾
uint32_t DescriptorPool::GetCount() const
{
    return m_RefCount;
}

// �f�B�X�N���v�^�n���h�������蓖�Ă�
DescriptorHandle* DescriptorPool::AllocHandle()
{
    // �������֐�
    auto func = [&](uint32_t index, DescriptorHandle* pHandle)
    {
        auto handleCPU = m_pHeap->GetCPUDescriptorHandleForHeapStart();
        handleCPU.ptr += m_DescriptorSize * index;

        auto handleGPU = m_pHeap->GetGPUDescriptorHandleForHeapStart();
        handleGPU.ptr += m_DescriptorSize * index;

        pHandle->HandleCPU = handleCPU;
        pHandle->HandleGPU = handleGPU;
    };

    // �������֐������s���Ă���n���h����ԋp
    return m_Pool.Alloc(func);
}

// �f�B�X�N���v�^�n���h�����������
void DescriptorPool::FreeHandle(DescriptorHandle*& pHandle)
{
    if (pHandle != nullptr)
    {
        // �n���h�����v�[���ɖ߂�
        m_Pool.Free(pHandle);

        // nullptr�ŃN���A
        pHandle = nullptr;
    }
}

// ���p�\�ȃn���h�������擾
uint32_t DescriptorPool::GetAvailableHandleCount() const
{
    return m_Pool.GetAvailableCount();
}

// ���蓖�čς݂̃n���h�����擾
uint32_t DescriptorPool::GetAllocatedHandleCount() const
{
    return m_Pool.GetUsedCount();
}

// �n���h���������擾
uint32_t DescriptorPool::GetHandleCount() const
{
    return m_Pool.GetSize();
}

// �f�B�X�N���v�^�q�[�v���擾
ID3D12DescriptorHeap* const DescriptorPool::GetHeap() const
{
    return m_pHeap.Get();
}

DescriptorPool::DescriptorPool()
    : m_RefCount(1)
    , m_Pool()
    , m_pHeap()
    , m_DescriptorSize(0)
{

}

DescriptorPool::~DescriptorPool()
{
    m_Pool.Term();
    m_pHeap.Reset();
    m_DescriptorSize = 0;
}
