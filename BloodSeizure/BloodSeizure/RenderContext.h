#pragma once
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "RootSignature.h"
#include "PipelineState.h"

/// <summary>
/// �����_�����O�R���e�L�X�g
/// </summary>
class RenderContext
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    RenderContext(){}

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~RenderContext(){}

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
    void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW& vb)
    {
        /*auto vertexBufferDesc = vb.GetView();*/
        m_pCommandList->IASetVertexBuffers(0, 1, &vb);
    }

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�̐ݒ�
    /// </summary>
    /// <param name="ib">�C���f�b�N�X�o�b�t�@</param>
    void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW& ib)
    {
        /*auto indexBufferDesc = ib.GetView();*/
        m_pCommandList->IASetIndexBuffer(&ib);
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

    /// <summary>
    /// �r���[�|�[�g�ƃV�U�����O��`��ݒ�
    /// </summary>
    /// <param name="viewport">�r���[�|�[�g</param>
    void SetViewportAndScissor(D3D12_VIEWPORT& viewport)
    {
        // �V�U�����O��`�ݒ�
        D3D12_RECT rect;
        rect.bottom = static_cast<LONG>(viewport.Height);
        rect.top = 0;
        rect.left = 0;
        rect.right = static_cast<LONG>(viewport.Width);
        SetScissorRect(rect);

        m_pCommandList->RSSetViewports(1, &viewport);
        m_currentViewport = viewport;
    }

    /// <summary>
    /// �r���[�|�[�g���擾�B
    /// </summary>
    /// <returns></returns>
    D3D12_VIEWPORT GetViewport() const
    {
        return m_currentViewport;
    }

    /// <summary>
    /// �V�U�����O��`��ݒ�
    /// </summary>
    /// <param name="rect"></param>
    void SetScissorRect(D3D12_RECT& rect)
    {
        m_pCommandList->RSSetScissorRects(1, &rect);
    }

    /// <summary>
    /// ���[�g�V�O�l�`����ݒ�B
    /// </summary>
    void SetRootSignature(ID3D12RootSignature* rootSignature)
    {
        m_pCommandList->SetGraphicsRootSignature(rootSignature);
    }
    //void SetRootSignature(RootSignature& rootSignature)
    //{
    //    auto ptrRootSig = rootSignature.Get();
    //    m_pCommandList->SetGraphicsRootSignature(rootSignature.Get());
    //}

    /// <summary>
    /// �p�C�v���C���X�e�[�g�̐ݒ�
    /// </summary>
    void SetPipelineState(ID3D12PipelineState* pipelineState)
    {
        m_pCommandList->SetPipelineState(pipelineState);
    }
    //void SetPipelineState(PipelineState& pipelineState)
    //{
    //    auto PSO = pipelineState.Get();
    //    m_pCommandList->SetPipelineState(pipelineState.Get());
    //}

    /// <summary>
    /// �C���f�b�N�X�t���v���~�e�B�u�`��
    /// </summary>
    /// <param name="indexCount"></param>
    void DrawIndexed(UINT indexCount)
    {
        m_pCommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
    }

    /// <summary>
    /// �C���X�^���V���O�`��
    /// </summary>
    /// <param name="indexCount"></param>
    /// <param name="numInstance"></param>
    void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
    {
        m_pCommandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
    }

private:
    enum { MAX_DESCRIPTOR_HEAP = 4 };   // �f�B�X�N���v�^�q�[�v�̍ő吔
    enum { MAX_CONSTANT_BUFFER = 8 };   // �萔�o�b�t�@�̍ő吔
    enum { MAX_SHADER_RESOURCE = 16 };  // �V�F�[�_�[���\�[�X�̍ő吔

    D3D12_VIEWPORT m_currentViewport;                                       // ���݂̃r���[�|�[�g�̐ݒ�
    ID3D12GraphicsCommandList* m_pCommandList;                              // �R�}���h���X�g
    ID3D12DescriptorHeap* m_pDescriptorHeaps[MAX_DESCRIPTOR_HEAP];          // �f�B�X�N���v�^�q�[�v�̔z��
    //ConstantBuffer* m_pConstantBuffers[MAX_CONSTANT_BUFFER];                // �萔�o�b�t�@�̔z��
    //Texture* m_pTextures[MAX_SHADER_RESOURCE];                              // �V�F�[�_�[���\�[�X�̔z��
    std::vector<ComPtr<ID3D12Resource>> m_scratchResourceList;              // �X�N���b�`���\�[�X
};