#pragma once

#include "GraphicsEngine.h"

/// <summary>
/// �C���f�b�N�X�o�b�t�@�[
/// </summary>
class IndexBuffer
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    IndexBuffer();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~IndexBuffer();

    /// <summary>
    /// ����������
    /// </summary>
    /// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y</param>
    /// <param name="stride">�X�g���C�h</param>
    void Init(int size, int stride);

    /// <summary>
    /// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�[�ɃR�s�[
    /// </summary>
    /// <param name="srcIndices">�R�s�[���̃C���f�b�N�X�f�[�^</param>
    void Copy(uint16_t* srcIndices);

    /// <summary>
    /// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�[�ɃR�s�[
    /// </summary>
    /// <param name="srcIndices">�R�s�[���̃C���f�b�N�X�f�[�^</param>
    void Copy(uint32_t* srcIndices);
    
    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�[���擾
    /// </summary>
    /// <returns></returns>
    D3D12_INDEX_BUFFER_VIEW GetView() const;

    /// <summary>
    /// �C���f�b�N�X�̐����擾
    /// </summary>
    /// <returns></returns>
    int GetCount() const;

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�̃X�g���C�h���擾
    /// </summary>
    /// <returns></returns>
    UINT GetStrideInBytes() const;

    /// <summary>
    /// ID3D12Resource�̃A�h���X���擾
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* GetID3D12ResourceAddress() const;

private:
    ID3D12Resource* m_pIndexBuffer = nullptr;       // �C���f�b�N�X�o�b�t�@�[
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;      // �C���f�b�N�X�o�b�t�@�[�r���[
    int m_count = 0;                                // �C���f�b�N�X�̐�
    int m_strideInBytes = 0;                        // �X�g���C�h�i�P�ʁF�o�C�g�j
};