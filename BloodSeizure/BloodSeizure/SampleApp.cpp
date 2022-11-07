#include "SampleApp.h"
#include "FileUtil.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"
#include "SimpleMath.h"
#include "ReleaseProcess.h"

using namespace DirectX::SimpleMath;

struct Transform
{
    Matrix  World;      // ���[���h�s��
    Matrix  View;       // �r���[�s��
    Matrix  Proj;       // �ˉe�s��
};

struct LightBuffer
{
    Vector4 LightPosition;  // ���C�g�ʒu
    Color   LightColor;     // ���C�g�J���[
};

struct MaterialBuffer
{
    Vector3 Diffuse;    // �g�U���˗�
    float   Alpha;      // ���ߓx
};

// �R���X�g���N�^
SampleApp::SampleApp(uint32_t width, uint32_t height)
    : App(width,height)
    , m_RotateAngle(0.0f)
{
}

// �f�X�g���N�^
SampleApp::~SampleApp()
{
}

// ����������
bool SampleApp::OnInit()
{
    // ���b�V�����[�h
    {
        std::wstring path;

        // �t�@�C���p�X����
        if (!SearchFilePath(L"data/teapot.obj", path))
        {
            return false;
        }

        std::wstring dir = GetDirectoryPath(path.c_str());

        std::vector<ResMesh> resMesh;
        std::vector<ResMaterial> resMaterial;

        // ���b�V�����\�[�X�����[�h
        if (!LoadMesh(path.c_str(), resMesh, resMaterial))
        {
            return false;
        }

        // ��������\��
        m_pMesh.resize(resMesh.size());

        // ���b�V��������
        for (size_t i = 0; i < resMesh.size(); ++i)
        {
            // ���b�V���𐶐�
            auto mesh = new Mesh;

            // �`�F�b�N
            if (mesh == nullptr)
            {
                return false;
            }

            // ����������
            if (!mesh->Init(m_pDevice.Get(), resMesh[i]))
            {
                return false;
            }

            // ����������o�^
            m_pMesh.push_back(mesh);
        }

        // ���������œK��
        m_pMesh.shrink_to_fit();

        // �}�e���A����������
        if (!m_Material.Init(m_pDevice.Get(),
            m_pPool[POOL_TYPE_RES],
            sizeof(MaterialBuffer),
            resMaterial.size()))
        {
            return false;
        }

        // ���\�[�X�o�b�`��p��
        DirectX::ResourceUploadBatch batch(m_pDevice.Get());

        // �o�b�`�J�n
        batch.Begin();

        // �e�N�X�`���ƃ}�e���A���ݒ�
        for (size_t i = 0; i < resMaterial.size(); ++i)
        {
            auto ptr = m_Material.GetBufferPtr<MaterialBuffer>(i);
            ptr->Diffuse  = resMaterial[i].Diffuse;
            ptr->Alpha    = resMaterial[i].Alpha;

            std::wstring path = dir + resMaterial[i].DiffuseMap;
            m_Material.SetTexture(i, TU_DIFFUSE, path, batch);
        }

        // �o�b�`�I��
        auto future = batch.End(m_pQueue.Get());

        // �o�b�`�����ҋ@
        future.wait();
    }

    // ���C�g�o�b�t�@�̐ݒ�
    {
        auto pCB = new ConstantBuffer;
        if (pCB == nullptr)
        {
            return false;
        }

        if (!pCB->Init(m_pDevice.Get(), m_pPool[POOL_TYPE_RES], sizeof(LightBuffer)))
        {
            return false;
        }

        auto ptr = pCB->GetPtr<LightBuffer>();
        ptr->LightPosition = Vector4(0.0f, 50.0f, 100.0f, 0.0f);
        ptr->LightColor    = Color(1.0f, 1.0f, 1.0f, 0.0f);

        m_pLight = pCB;
    }

    // ���[�g�V�O�j�`���̐���
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        // �f�B�X�N���v�^�����W��ݒ�
        D3D12_DESCRIPTOR_RANGE range = {};
        range.RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors                    = 1;
        range.BaseShaderRegister                = 0;
        range.RegisterSpace                     = 0;
        range.OffsetInDescriptorsFromTableStart = 0;

        // ���[�g�p�����[�^�ݒ�
        D3D12_ROOT_PARAMETER param[4] = {};
        param[0].ParameterType              = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[0].Descriptor.ShaderRegister  = 0;
        param[0].Descriptor.RegisterSpace   = 0;
        param[0].ShaderVisibility           = D3D12_SHADER_VISIBILITY_VERTEX;

        param[1].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[1].Descriptor.ShaderRegister   = 1;
        param[1].Descriptor.RegisterSpace    = 0;
        param[1].ShaderVisibility            = D3D12_SHADER_VISIBILITY_PIXEL;

        param[2].ParameterType               = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[2].Descriptor.ShaderRegister   = 2;
        param[2].Descriptor.RegisterSpace    = 0;
        param[2].ShaderVisibility            = D3D12_SHADER_VISIBILITY_PIXEL;

        param[3].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param[3].DescriptorTable.NumDescriptorRanges = 1;
        param[3].DescriptorTable.pDescriptorRanges   = &range;
        param[3].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_PIXEL;

        // �X�^�e�B�b�N�T���v���[�̐ݒ�
        auto sampler = DirectX::CommonStates::StaticLinearWrap(0, D3D12_SHADER_VISIBILITY_PIXEL);

        // ���[�g�V�O�j�`��
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters       = _countof(param);
        desc.NumStaticSamplers   = 1;
        desc.pParameters         = param;
        desc.pStaticSamplers     = &sampler;
        desc.Flags               = flag;

        ComPtr<ID3DBlob> pBlob;
        ComPtr<ID3DBlob> pErrorBlob;

        // �V���A���C�Y
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf()
        );
        if (FAILED(hr))
        {
            return false;
        }

        // ���[�g�V�O�l�`���𐶐�
        hr = m_pDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(m_pRootSig.GetAddressOf())
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �p�C�v���C���X�e�[�g
    {
        std::wstring vsPath;
        std::wstring psPath;

        // ���_�V�F�[�_������
        if (!SearchFilePath(L"LambertVS.cso", vsPath))
        {
            return false;
        }

        // �s�N�Z���V�F�[�_������
        if (!SearchFilePath(L"LambertPS.cso", psPath))
        {
            return false;
        }

        ComPtr<ID3DBlob> pVSBlob;
        ComPtr<ID3DBlob> pPSBlob;

        // ���_�V�F�[�_��Ǎ���
        auto hr = D3DReadFileToBlob(vsPath.c_str(), pVSBlob.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        // �s�N�Z���V�F�[�_��Ǎ���
        hr = D3DReadFileToBlob(psPath.c_str(), pPSBlob.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        // �O���t�B�b�N�X�p�C�v���C���X�e�[�g��ݒ�
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout             = MeshVertex::InputLayout;
        desc.pRootSignature          = m_pRootSig.Get();
        desc.VS                      = { pVSBlob->GetBufferPointer(),pVSBlob->GetBufferSize() };
        desc.PS                      = { pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize() };
        desc.RasterizerState         = DirectX::CommonStates::CullNone;
        desc.BlendState              = DirectX::CommonStates::Opaque;
        desc.DepthStencilState       = DirectX::CommonStates::DepthDefault;
        desc.SampleMask              = UINT_MAX;
        desc.PrimitiveTopologyType   = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets        = 1;
        desc.RTVFormats[0]           = m_ColorTarget[0].GetViewDesc().Format;
        desc.DSVFormat               = m_DepthTarget.GetViewDesc().Format;
        desc.SampleDesc.Count        = 1;
        desc.SampleDesc.Quality      = 0;

        // �p�C�v���C���X�e�[�g�𐶐�
        hr = m_pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �ϊ��s��p�̒萔�o�b�t�@�̐���
    {
        m_Transform.reserve(FrameCount);

        for (auto i = 0u; i < FrameCount; ++i)
        {
            auto pCB = new ConstantBuffer;
            if (pCB == nullptr)
            {
                return false;
            }

            // �萔�o�b�t�@������
            if (!pCB->Init(m_pDevice.Get(), m_pPool[POOL_TYPE_RES], sizeof(Transform) * 2))
            {
                return false;
            }

            // �J�����ݒ�
            auto eyePos = Vector3(0.0f, 1.0f, 2.0f);
            auto targetPos = Vector3::Zero;
            auto upward = Vector3::UnitY;

            // ������p�ƃA�X�y�N�g��̐ݒ�
            auto fovY = DirectX::XMConvertToRadians(37.5f);
            auto aspect = static_cast<float>(m_Width) / static_cast<float>(m_Height);

            // �ϊ��s���ݒ�
            auto ptr = pCB->GetPtr<Transform>();
            ptr->World = Matrix::Identity;
            ptr->Proj = Matrix::CreateLookAt(eyePos, targetPos, upward);
            ptr->Proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, 1.0f, 1000.0f);

            m_Transform.push_back(pCB);
        }

        m_RotateAngle = DirectX::XMConvertToRadians(-60.0f);
    }
    return true;
}

// �������
void SampleApp::OnTerm()
{
    // ���b�V���j��
    for (size_t i = 0; i < m_pMesh.size(); ++i)
    {
        SafeTerm(m_pMesh[i]);
    }
    m_pMesh.clear();
    m_pMesh.shrink_to_fit();

    // �}�e���A���j��
    m_Material.Term();

    // ���C�g�j��
    SafeDelete(m_pLight);

    // �ϊ��o�b�t�@�j��
    for (size_t i = 0; i < m_Transform.size(); ++i)
    {
        SafeTerm(m_Transform[i]);
    }
    m_Transform.clear();
    m_Transform.shrink_to_fit();
}

// �`�揈��
void SampleApp::OnRender()
{
}
