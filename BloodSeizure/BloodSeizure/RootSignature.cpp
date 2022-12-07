#include "RootSignature.h"
#include "CommonStates.h"

void RootSignature::Init(D3D12_ROOT_PARAMETER* param,UINT paramNum)
{
    auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
    flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
    flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    // �X�^�e�B�b�N�T���v���[�̐ݒ�
    auto sampler = DirectX::CommonStates::StaticLinearWrap(0, D3D12_SHADER_VISIBILITY_PIXEL);

    // ���[�g�V�O�j�`��
    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters      = paramNum;
    desc.NumStaticSamplers  = 1;
    desc.pParameters        = param;
    desc.pStaticSamplers    = &sampler;
    desc.Flags              = flag;

    ComPtr<ID3DBlob> pBlob;
    ComPtr<ID3DBlob> pErrorBlob;

    // �V���A���C�Y
    D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        pBlob.GetAddressOf(),
        pErrorBlob.GetAddressOf()
    );

    // ���[�g�V�O�j�`���𐶐�
    auto device = g_graphicsEngine->GetD3DDevice();
    device->CreateRootSignature(
        0,
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        IID_PPV_ARGS(m_pRootSignature.GetAddressOf())
    );
}
