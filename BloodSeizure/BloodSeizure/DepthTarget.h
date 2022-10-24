#pragma once

#include <d3d12.h>
#include <cstdint>
#include "ComPtr.h"

class DescriptorHandle;
class DescriptorPool;

class DepthTarget
{
public:
    DepthTarget();
    ~DepthTarget();

    // ����������
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPoolDSV, uint32_t width, uint32_t height, DXGI_FORMAT format);

    // �������
    void Term();

    // �f�B�X�N���v�^�n���h���iDSV�p�j���擾
    DescriptorHandle* GetHandleDSV() const;

    // ���\�[�X���擾
    ID3D12Resource* GetResource() const;

    // ���\�[�X�ݒ���擾
    D3D12_RESOURCE_DESC GetDesc() const;

    // �[�x�X�e���V���r���[�̐ݒ���擾
    D3D12_DEPTH_STENCIL_VIEW_DESC GetViewDesc() const;

private:
    ComPtr<ID3D12Resource>          m_pTarget;      // ���\�[�X
    DescriptorHandle*               m_pHandleDSV;   // �f�B�X�N���v�^�n���h��
    DescriptorPool*                 m_pPoolDSV;     // �f�B�X�N���v�^�v�[��
    D3D12_DEPTH_STENCIL_VIEW_DESC   m_ViewDesc;     // �[�x�X�e���V���r���[�̐ݒ�

    DepthTarget(const DepthTarget&) = delete;
    void operator = (const DepthTarget&) = delete;
};