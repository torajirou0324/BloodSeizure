#include "Texture.h"
#include <DDSTextureLoader.h>

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetViewDesc(bool isCube)
{
    auto desc = m_pTexture->GetDesc();
    D3D12_SHADER_RESOURCE_VIEW_DESC ViewDesc = {};

    ViewDesc.Format = desc.Format;
    ViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    switch (desc.Dimension)
    {
    case D3D12_RESOURCE_DIMENSION_BUFFER:
    {
        // バッファは対象外
        abort();  // アプリを止める
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
        // 想定外
        abort(); // アプリを止める
    }
        break;
    }
    return ViewDesc;
}

void Texture::InitDDSFile(const wchar_t* filePath)
{
    // テクスチャをロード
    LoadTextureFromDDSFile(filePath);
}

void Texture::InitMemory(const char* memory, unsigned int size)
{
    // テクスチャをロード
    LoadTextureFromMemory(memory, size);
}

void Texture::InitD3DResource(ID3D12Resource* texture)
{
    if (m_pTexture)
    {
        m_pTexture->Release();
    }
    m_pTexture = texture;
    m_pTexture->AddRef();
    m_textureDesc = m_pTexture->GetDesc();
}

void Texture::SetShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    if (m_pTexture)
    {
        auto device = g_graphicsEngine->GetD3DDevice();
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format                  = m_textureDesc.Format;
        srvDesc.ViewDimension           = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels     = m_textureDesc.MipLevels;
        device->CreateShaderResourceView(m_pTexture, &srvDesc, descriptorHandle);
    }
}

void Texture::LoadTextureFromDDSFile(const wchar_t* filePath)
{
    auto device = g_graphicsEngine->GetD3DDevice();
    DirectX::ResourceUploadBatch re(device);
    re.Begin();
    ID3D12Resource* texture;
    auto hr = DirectX::CreateDDSTextureFromFileEx(
        device,
        re,
        filePath,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        DirectX::DDS_LOADER_FLAGS::DDS_LOADER_DEFAULT,
        &texture
    );
    re.End(g_graphicsEngine->GetCommandQueue());

    if (FAILED(hr))
    {
        return;
    }

    m_pTexture = texture;
    m_textureDesc = m_pTexture->GetDesc();
}

void Texture::LoadTextureFromMemory(const char* memory, unsigned int size)
{
    auto device = g_graphicsEngine->GetD3DDevice();
    DirectX::ResourceUploadBatch re(device);
    re.Begin();
    ID3D12Resource* texture;
    auto hr = DirectX::CreateDDSTextureFromMemoryEx(
        device,
        re,
        (const uint8_t*)memory,
        size,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        DirectX::DDS_LOADER_DEFAULT,
        &texture
    );

    re.End(g_graphicsEngine->GetCommandQueue());
    if (FAILED(hr))
    {
        return;
    }

    m_pTexture = texture;
    m_textureDesc = m_pTexture->GetDesc();
}
