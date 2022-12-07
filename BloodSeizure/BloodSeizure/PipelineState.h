#pragma once
#include "GraphicsEngine.h"
#include "RootSignature.h"
#include "Shader.h"

class PipelineState
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PipelineState(){}
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~PipelineState(){}

    /// <summary>
    /// �p�C�v���C���X�e�[�g�̏�����
    /// </summary>
    /// <param name="pipelineState">�p�C�v���C���X�e�[�g</param>
    /// <param name="rs">���[�g�V�O�l�`��</param>
    /// <param name="vs">�o�[�e�b�N�X�V�F�[�_�[</param>
    /// <param name="ps">�s�N�Z���V�F�[�_�[</param>
    void Init(D3D12_INPUT_ELEMENT_DESC* inputElements, UINT elementSize, RootSignature& rs, Shader& vs, Shader& ps);

    /// <summary>
    /// �p�C�v���C���X�e�[�g�̎擾
    /// </summary>
    /// <returns></returns>
    ID3D12PipelineState* Get()
    {
        return m_pPipelineState;
    }

private:
    ID3D12PipelineState* m_pPipelineState = nullptr;    // �p�C�v���C���X�e�[�g
};