#pragma once

#include <d3d12.h>
#include <atomic>
#include "ComPtr.h"
#include "Pool.h"

class DescriptorHandle
{
public:
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;  // CPU�f�B�X�N���v�^�n���h��
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;  // GPU�f�B�X�N���v�^�n���h��

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
    // ��������
    static bool Create(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* pDesc, DescriptorPool** ppPool);

    // �Q�ƃJ�E���g�𑝂₷
    void AddRef();

    // �������
    void Release();

    // �Q�ƃJ�E���g���擾
    uint32_t GetCount() const;

    // �f�B�X�N���v�^�n���h�������蓖�Ă�
    DescriptorHandle* AllocHandle();

    // �f�B�X�N���v�^�n���h�����������
    void FreeHandle(DescriptorHandle*& pHandle);

    // ���p�\�ȃn���h����Ԃ�
    uint32_t GetAvailableHandleCount() const;

    // ���蓖�čς݂̃n���h�����擾
    uint32_t GetAllocatedHandleCount() const;

    // �n���h�������擾
    uint32_t GetHandleCount() const;

    // �f�B�X�N���v�^�q�[�v���擾
    ID3D12DescriptorHeap* const GetHeap() const;

private:
    std::atomic<uint32_t>           m_RefCount;         // �Q�ƃJ�E���g
    Pool<DescriptorHandle>          m_Pool;             // �f�B�X�N���v�^�n���h���v�[��
    ComPtr<ID3D12DescriptorHeap>    m_pHeap;            // �f�B�X�N���v�^�q�[�v
    uint32_t                        m_DescriptorSize;   // �f�B�X�N���v�^�T�C�Y

    DescriptorPool();   // �R���X�g���N�^
    ~DescriptorPool();  // �f�X�g���N�^

    DescriptorPool(const DescriptorPool&) = delete;
    void operator = (const DescriptorPool&) = delete;
};