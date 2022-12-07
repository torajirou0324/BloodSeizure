#pragma once

#include <d3d12.h>
#include <ResourceUploadBatch.h>
#include "ComPtr.h"

class Texture
{
public:
    Texture();
    ~Texture();

    // ����������(batch)�e�N�X�`���̍X�V�ɕK�v�ȃf�[�^���i�[����
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, const wchar_t* filename, DirectX::ResourceUploadBatch& batch);

    // ����������(isCube)�L���[�u�}�b�v�ł���ꍇ��true���w��
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, const D3D12_RESOURCE_DESC* pDesc, bool isCube);

    // �������
    void Term();

    // CPU�f�B�X�N���v�^�n���h�����擾
    D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU() const;

    // GPU�f�B�X�N���v�^�n���h�����擾
    D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU() const;

private:
    ComPtr<ID3D12Resource>  m_pTex;
    DescriptorHandle*       m_pHandle;
    DescriptorPool*         m_pPool;

    Texture(const Texture&) = delete;
    void operator = (const Texture&) = delete;

    // �V�F�[�_�[���\�[�X�̐ݒ���擾
    D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool isCube);
};