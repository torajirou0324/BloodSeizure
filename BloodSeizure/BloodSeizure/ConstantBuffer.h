#pragma once
#include "GraphicsEngine.h"

class ConstantBuffer
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    ConstantBuffer();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~ConstantBuffer();

    /// <summary>
    /// ����������
    /// </summary>
    /// <param name="size">�萔�o�b�t�@�̃T�C�Y</param>
    /// <param name="srcData">�\�[�X�f�[�^�Bnull���w�肷�邱�Ƃ���</param>
    void Init(int size, void* srcData = nullptr);

    /// <summary>
    /// �������
    /// </summary>
    void Term();

    /// <summary>
    /// �f�B�X�N���v�^�q�[�v��ConstantBufferView��o�^�B
    /// </summary>
    /// <param name="descriptorHandle"></param>
    void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
    void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

    /// <summary>
    /// GPU���z�A�h���X���擾����
    /// </summary>
    /// <returns></returns>
    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();

    /// <summary>
    /// �f�[�^��VRAM�ɃR�s�[����
    /// </summary>
    /// <param name="data"></param>
    void CopyToVRAM(void* data);
    template<class T>
    void CopyToVRAM(T& data)
    {
        CopyToVRAM(&data)
    }

private:
    ComPtr<ID3D12Resource>          m_pConstantBuffer[2] = { nullptr };         // �萔�o�b�t�@
    void*                           m_pConstantBufferCPU[2] = { nullptr };      // �}�b�v�ς݃|�C���^
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_Desc;                                     // �萔�o�b�t�@�r���[�\����
    int                             m_size = 0;                                 // �萔�o�b�t�@�T�C�Y
    int                             m_alocSize = 0;                             // 
};