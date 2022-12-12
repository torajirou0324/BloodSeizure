#pragma once

#include <ResourceUploadBatch.h>
#include "GraphicsEngine.h"

class Texture
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    Texture(){}

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~Texture(){}

    /// <summary>
    /// DDS�t�@�C������e�N�X�`��������������
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X</param>
    void InitDDSFile(const wchar_t* filePath);

    /// <summary>
    /// ����������e�N�X�`��������������
    /// </summary>
    /// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă郁�����̐擪�A�h���X</param>
    /// <param name="size">�e�N�X�`���̃T�C�Y</param>
    void InitMemory(const char* memory, unsigned int size);

    /// <summary>
    /// D3D���\�[�X����e�N�X�`��������������
    /// </summary>
    /// <param name="texture">D3D���\�[�X</param>
    void InitD3DResource(ID3D12Resource* texture);

    /// <summary>
    /// �V�F�[�_�[���\�[�X�r���[��o�^(SRV)
    /// </summary>
    /// <param name="descriptorHandle"></param>
    /// <param name="bufferNo"></param>
    void SetShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

    /// <summary>
    /// �e�N�X�`���̃��\�[�X���擾
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* Get()
    {
        return m_pTexture;
    }

    /// <summary>
    /// �e�N�X�`���̕����擾
    /// </summary>
    /// <returns></returns>
    int GetWidth() const
    {
        return static_cast<int>(m_textureDesc.Width);
    }

    /// <summary>
    /// �e�N�X�`���̍������擾
    /// </summary>
    /// <returns></returns>
    int GetHeight() const
    {
        return static_cast<int>(m_textureDesc.Height);
    }

private:
    /// <summary>
    /// DDS�t�@�C������e�N�X�`�������[�h
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X</param>
    void LoadTextureFromDDSFile(const wchar_t* filePath);

    /// <summary>
    /// ����������e�N�X�`�������[�h
    /// </summary>
    /// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă郁�����̐擪�A�h���X</param>
    /// <param name="size">�e�N�X�`���T�C�Y</param>
    void LoadTextureFromMemory(const char* memory, unsigned int size);

    /// <summary>
    /// �V�F�[�_�[���\�[�X�̐ݒ���擾
    /// </summary>
    /// <param name="isCube">�L���[�u�}�b�v</param>
    /// <returns></returns>
    D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool isCube);

    ID3D12Resource* m_pTexture = nullptr;
    D3D12_RESOURCE_DESC m_textureDesc;
};