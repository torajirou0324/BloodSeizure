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

    // ����������
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, size_t size);

    // �������
    void Term();

    // GPU���z�A�h���X���擾����
    D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const;

    // CPU�f�B�X�N���v�^�n���h�����擾
    D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU() const;

    // GPU�f�B�X�N���v�^�n���h�����擾
    D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU() const;

    // �������}�b�s���O�ς݃|�C���^���擾
    void* GetPtr() const;

    // �������}�b�s���O�ς݃|�C���^���擾
    template<typename T>
    T* GetPtr()
    {
        return reinterpret_cast<T*>(GetPtr());
    }

private:
    ComPtr<ID3D12Resource>          m_pCB;          // �萔�o�b�t�@
    DescriptorHandle*               m_pHandle;      // �f�B�X�N���v�^�n���h��
    DescriptorPool*                 m_pPool;        // �f�B�X�N���v�^�v�[��
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_Desc;         // �萔�o�b�t�@�r���[�\����
    void*                           m_pMappedPtr;   // �}�b�v�ς݃|�C���^

    ConstantBuffer(const ConstantBuffer&) = delete;
    void operator = (const ConstantBuffer&) = delete;
};