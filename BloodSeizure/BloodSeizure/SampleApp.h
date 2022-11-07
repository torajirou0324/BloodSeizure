#pragma once
#include "App.h"
#include "ConstantBuffer.h"
#include "Material.h"

class SampleApp : public App
{
public:
    SampleApp(uint32_t width, uint32_t height);
    virtual ~SampleApp();

private:
    std::vector<Mesh*>              m_pMesh;        // ���b�V��
    std::vector<ConstantBuffer*>    m_Transform;   // �ϊ��s��
    ConstantBuffer*                 m_pLight;       // ���C�g
    Material                        m_Material;     // �}�e���A��
    ComPtr<ID3D12PipelineState>     m_pPSO;         // �p�C�v���C���X�e�[�g
    ComPtr<ID3D12RootSignature>     m_pRootSig;     // ���[�g�V�O�l�`��
    float                           m_RotateAngle;  // ��]�p�x

    // ����������
    bool OnInit() override;

    // �������
    void OnTerm() override;

    // �`�揈��
    void OnRender() override;
};