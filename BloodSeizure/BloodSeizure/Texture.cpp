#include "Texture.h"
#include <DDSTextureLoader.h>
#include "DescriptorPool.h"

// �R���X�g���N�^
Texture::Texture()
    : m_pTex(nullptr)
    , m_pHandle(nullptr)
    , m_pPool(nullptr)
{
}

// �f�X�g���N�^
Texture::~Texture()
{
    Term();
}

// ����������
bool Texture::Init(ID3D12Device* pDevice, DescriptorPool* pPool, const wchar_t* filename, DirectX::ResourceUploadBatch& batch)
{
    // �����`�F�b�N
    if (pDevice == nullptr || pPool == nullptr || filename == nullptr)
    {
        return false;
    }

    assert(m_pPool == nullptr);
    assert(m_pHandle == nullptr);

    // �f�B�X�N���v�^�v�[�����w��
    m_pPool = pPool;
    m_pPool->AddRef();

    // �f�B�X�N���v�^�n���h�����擾
    m_pHandle = pPool->AllocHandle();
    if (m_pHandle == nullptr)
    {
        return false;
    }

    // �t�@�C������e�N�X�`���𐶐�
    bool isCube = false;
    auto hr = DirectX::CreateDDSTextureFromFile(
        pDevice,
        batch,
        filename,
        m_pTex.GetAddressOf(),
        true,
        0,
        nullptr,
        &isCube
    );
    if (FAILED(hr))
    {
        return false;
    }

    // �V�F�[�_�[���\�[�X�r���[�̐ݒ���擾
    auto ViewDesc = GetViewDesc(isCube);

    // �V�F�[�_�[���\�[�X�r���[�𐶐�
    pDevice->CreateShaderResourceView(m_pTex.Get(), &ViewDesc, m_pHandle->HandleCPU);

    // ����I��
    return true;
}

bool Texture::Init(ID3D12Device* pDevice, DescriptorPool* pPool, const D3D12_RESOURCE_DESC* pDesc, bool isCube)
{
    // �����`�F�b�N
    if (pDevice == nullptr || pPool == nullptr || pDesc == nullptr)
    {
        return false;
    }

    assert(m_pPool == nullptr);
    assert(m_pHandle == nullptr);

    // �f�B�X�N���v�^�v�[����ݒ�
    m_pPool = pPool;
    m_pPool->AddRef();

    // �f�B�X�N���v�^�n���h�����擾
    m_pHandle = pPool->AllocHandle();
    if (m_pHandle == nullptr)
    {
        return false;
    }

    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_DEFAULT;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.CreationNodeMask = 0;
    prop.VisibleNodeMask = 0;

    auto hr = pDevice->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        pDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(m_pTex.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

    // �V�F�[�_�[���\�[�X�r���[�̐ݒ�
    auto ViewDesc = GetViewDesc(isCube);

    // �V�F�[�_�[���\�[�X�r���[�𐶐�
    pDevice->CreateShaderResourceView(m_pTex.Get(), &ViewDesc, m_pHandle->HandleCPU);

    // ����I��
    return true;
}

// �������
void Texture::Term()
{
    m_pTex.Reset();

    // �f�B�X�N���v�^�n���h�������
    if (m_pHandle != nullptr && m_pPool != nullptr)
    {
        m_pPool->FreeHandle(m_pHandle);
        m_pHandle = nullptr;
    }

    // �f�B�X�N���v�v�[�������
    if (m_pPool != nullptr)
    {
        m_pPool->Release();
        m_pPool = nullptr;
    }
}

// CPU�f�B�X�N���v�^�n���h�����擾
D3D12_CPU_DESCRIPTOR_HANDLE Texture::GetHandleCPU() const
{
    if (m_pHandle != nullptr)
    {
        return m_pHandle->HandleCPU;
    }
    return D3D12_CPU_DESCRIPTOR_HANDLE();
}

// GPU�f�B�X�N���v�^�n���h�����擾
D3D12_GPU_DESCRIPTOR_HANDLE Texture::GetHandleGPU() const
{
    if (m_pHandle != nullptr)
    {
        m_pHandle->HandleGPU;
    }
    return D3D12_GPU_DESCRIPTOR_HANDLE();
}

// �V�F�[�_�[���\�[�X�̐ݒ���擾
D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetViewDesc(bool isCube)
{
    auto desc = m_pTex->GetDesc();
    D3D12_SHADER_RESOURCE_VIEW_DESC ViewDesc = {};

    ViewDesc.Format = desc.Format;
    ViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    switch (desc.Dimension)
    {
    case D3D12_RESOURCE_DIMENSION_BUFFER:
    {
        // �o�b�t�@�͑ΏۊO
        abort();  // �A�v�����~�߂�
    }
    break;
    case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
    {
        if (desc.DepthOrArraySize > 1)
        {
            ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;

            ViewDesc.Texture1DArray.MostDetailedMip = 0;
            ViewDesc.Texture1DArray.MipLevels = desc.MipLevels;
            ViewDesc.Texture1DArray.FirstArraySlice = 0;
            ViewDesc.Texture1DArray.ArraySize = desc.DepthOrArraySize;
            ViewDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
        }
        else
        {
            ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;

            ViewDesc.Texture1D.MostDetailedMip = 0;
            ViewDesc.Texture1D.MipLevels = desc.MipLevels;
            ViewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
        }
        break;
    }
    case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
    {
        if (isCube)
        {
            if (desc.DepthOrArraySize > 6)
            {
                ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;

                ViewDesc.TextureCubeArray.MostDetailedMip = 0;
                ViewDesc.TextureCubeArray.MipLevels = desc.MipLevels;
                ViewDesc.TextureCubeArray.First2DArrayFace = 0;
                ViewDesc.TextureCubeArray.NumCubes = (desc.DepthOrArraySize / 6);
                ViewDesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
            }
            else
            {
                ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;

                ViewDesc.TextureCube.MostDetailedMip = 0;
                ViewDesc.TextureCube.MipLevels = desc.MipLevels;
                ViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
            }
        }
        else
        {
            if (desc.DepthOrArraySize > 1)
            {
                if (desc.MipLevels > 1)
                {
                    ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;

                    ViewDesc.Texture2DMSArray.FirstArraySlice = 0;
                    ViewDesc.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
                }
                else
                {
                    ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

                    ViewDesc.Texture2DArray.MostDetailedMip = 0;
                    ViewDesc.Texture2DArray.MipLevels = desc.MipLevels;
                    ViewDesc.Texture2DArray.FirstArraySlice = 0;
                    ViewDesc.Texture2DArray.ArraySize = desc.DepthOrArraySize;
                    ViewDesc.Texture2DArray.PlaneSlice = 0;
                    ViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                }
            }
            else
            {
                if (desc.MipLevels > 1)
                {
                    ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                }
                else
                {
                    ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

                    ViewDesc.Texture2D.MostDetailedMip = 0;
                    ViewDesc.Texture2D.MipLevels = desc.MipLevels;
                    ViewDesc.Texture2D.PlaneSlice = 0;
                    ViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
                }
            }
        }
        break;
    }
    case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
    {
        ViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;

        ViewDesc.Texture3D.MostDetailedMip = 0;
        ViewDesc.Texture3D.MipLevels = desc.MipLevels;
        ViewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
    }
    default:
    {
        // �z��O
        abort(); // �A�v�����~�߂�
    }
        break;
    }
    return ViewDesc;
}
