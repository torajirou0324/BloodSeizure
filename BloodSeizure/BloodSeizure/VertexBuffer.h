#pragma once

#include "GraphicsEngine.h"

/// <summary>
/// ���_�o�b�t�@
/// </summary>
class VertexBuffer
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    VertexBuffer();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~VertexBuffer();

    /// <summary>
    /// ����������
    /// </summary>
    /// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
    /// <param name="stride">���_��̃T�C�Y</param>
    void Init(void* size, int stride);

    /// <summary>
    /// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[
    /// </summary>
    /// <param name="srcVertices">�R�s�[���̒��_�f�[�^</param>
    //void Copy(void* srcVertices);

    /// <summary>
    /// ID3D12Resource�̃A�h���X���擾
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* GetID3DResourceAddress() const;

    /// <summary>
    /// ���_�o�b�t�@�r���[���擾
    /// </summary>
    /// <returns></returns>
    D3D12_VERTEX_BUFFER_VIEW GetView() const;

private:
    ID3D12Resource* m_pVertexBuffer = nullptr;      // ���_�o�b�t�@
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;    // ���_�o�b�t�@�r���[
};