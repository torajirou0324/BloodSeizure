#pragma once
#include <vector>
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Texture;

/// <summary>
/// �����_�����O�R���e�L�X�g
/// </summary>
class RenderContext
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    RenderContext();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~RenderContext();

    /// <summary>
    /// �������B
    /// </summary>
    /// <param name="commandList">�R�}���h���X�g</param>
    void Init(ID3D12GraphicsCommandList* commandList)
    {
        m_pCommandList = commandList;
    }

    /// <summary>
    /// ���_�o�b�t�@�̐ݒ�
    /// </summary>
    /// <param name="vb">���_�o�b�t�@</param>
    void SetVertexBuffer(VertexBuffer& vb)
    {
        m_pCommandList->IASetVertexBuffers(0, 1, &vb.GetView());
    }

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�̐ݒ�
    /// </summary>
    /// <param name="ib">�C���f�b�N�X�o�b�t�@</param>
    void SetIndexBuffer(IndexBuffer& ib)
    {
        m_pCommandList->IASetIndexBuffer(&ib.GetView());
    }

    /// <summary>
    /// �v���~�e�B�u�g�|���W�[��ݒ�
    /// </summary>
    /// <param name="topology">�v���~�e�B�u�g�|���W�[�̃^�C�v</param>
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
    {
        m_pCommandList->IASetPrimitiveTopology(topology);
    }

    /// <summary>
    /// �R�}���h���X�g��ݒ�
    /// </summary>
    /// <param name="commandList">�R�}���h���X�g</param>
    void SetCommandList(ID3D12GraphicsCommandList* commandList)
    {
        m_pCommandList = commandList;
    }

    void SetPipelineState()
    {

    }

private:
    enum { MAX_DESCRIPTOR_HEAP = 4 };   // �f�B�X�N���v�^�q�[�v�̍ő吔
    enum { MAX_CONSTANT_BUFFER = 8 };   // �萔�o�b�t�@�̍ő吔
    enum { MAX_SHADER_RESOURCE = 16 };  // �V�F�[�_�[���\�[�X�̍ő吔

    D3D12_VIEWPORT m_currentViewport;                                       // ���݂̃r���[�|�[�g�̐ݒ�
    ID3D12GraphicsCommandList* m_pCommandList;                              // �R�}���h���X�g
    ID3D12DescriptorHeap* m_pDescriptorHeaps[MAX_DESCRIPTOR_HEAP];          // �f�B�X�N���v�^�q�[�v�̔z��
    ConstantBuffer* m_pConstantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };  // �萔�o�b�t�@�̔z��
    Texture* m_pTextures[MAX_SHADER_RESOURCE] = { nullptr };                // �V�F�[�_�[���\�[�X�̔z��
    std::vector<ComPtr<ID3D12Resource>> m_scratchResourceList;              // �X�N���b�`���\�[�X
};