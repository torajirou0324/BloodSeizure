#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdint>
#include "ComPtr.h"

class DescriptorHandle;
class DescriptorPool;

class ColorTarget
{
public:
    ColorTarget();
    ~ColorTarget();

    // ����������
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t width, uint32_t height, DXGI_FORMAT format);

    // �o�b�N�o�b�t�@���珉�������������s
    bool InitFromBackBuffer(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t index, IDXGISwapChain* pSwapChain);

    // �������
    void Term();

    // �f�B�X�N���v�^�n���h���iRTV�p�j���擾
    DescriptorHandle* GetHandleRTV() const;

    // ���\�[�X���擾
    ID3D12Resource* GetResource() const;

    // ���\�[�X�ݒ���擾
    D3D12_RESOURCE_DESC GetDesc() const;

    // �����_�[�^�[�Q�b�g�r���[�̐ݒ���擾
    D3D12_RENDER_TARGET_VIEW_DESC GetViewDesc() const;

private:
    ComPtr<ID3D12Resource>          m_pTarget;      // ���\�[�X
    DescriptorHandle*               m_pHandleRTV;   // �f�B�X�N���v�^�n���h���iRTV�p�j
    DescriptorPool*                 m_pPoolRTV;     // �f�B�X�N���v�^�v�[���iRTV�p�j
    D3D12_RENDER_TARGET_VIEW_DESC   m_ViewDesc;     // �����_�[�^�[�Q�b�g�r���[�\����

    ColorTarget(const ColorTarget&) = delete;
    void operator = (const ColorTarget&) = delete;
};