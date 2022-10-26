#include "Material.h"
#include "FileUtil.h"

Material::Material()
    : m_pDevice(nullptr)
    , m_pPool(nullptr)
{
}

Material::~Material()
{
    Term();
}

bool Material::Init(ID3D12Device* pDevice,DescriptorPool* pPool,size_t bufferSize,size_t count)
{
    // 引数チェック
    if (pDevice == nullptr || pPool == nullptr || count == 0)
    {
        return false;
    }

    Term();

    m_pDevice = pDevice;
    m_pDevice->AddRef();

    m_pPool = pPool;
    m_pPool->AddRef();

    m_Subset.resize(count);

    // ダミーテクスチャ生成
    {
        auto pTexture = new (std::nothrow) Texture;
        if (pTexture == nullptr)
        {
            return false;
        }

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Width = 1;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

        if (!pTexture->Init(
            pDevice,
            pPool,
            &desc,
            false))
        {
            pTexture->Term();
            delete pTexture;
            return false;
        }

        m_pTexture[L""] = pTexture;
    }

    auto size = bufferSize * count;
    if (size > 0)
    {
        for (size_t i = 0; i < m_Subset.size(); ++i)
        {
            auto pBuffer = new (std::nothrow) ConstantBuffer();
            if (pBuffer == nullptr)
            {
                return false;
            }

            if (!pBuffer->Init(
                pDevice,
                pPool,
                bufferSize))
            {
                return false;
            }

            m_Subset[i].pConstantBuffer = pBuffer;
            for (auto j = 0; j < TEXTURE_USAGE_COUNT; ++j)
            {
                m_Subset[i].TextureHandle[i].ptr = 0;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < m_Subset.size(); ++i)
        {
            m_Subset[i].pConstantBuffer = nullptr;
            for (auto j = 0; j < TEXTURE_USAGE_COUNT; ++j)
            {
                m_Subset[i].TextureHandle[i].ptr = 0;
            }
        }
    }

    // 正常終了
    return true;
}

// 終了処理
void Material::Term()
{
    for (auto& itr : m_pTexture)
    {
        if (itr.second != nullptr)
        {
            itr.second->Term();
            delete itr.second;
            itr.second = nullptr;
        }
    }

    for (size_t i = 0; i < m_Subset.size(); ++i)
    {
        if (m_Subset[i].pConstantBuffer != nullptr)
        {
            m_Subset[i].pConstantBuffer->Term();
            delete m_Subset[i].pConstantBuffer;
            m_Subset[i].pConstantBuffer = nullptr;
        }
    }

    m_pTexture.clear();
    m_Subset.clear();

    if (m_pDevice != nullptr)
    {
        m_pDevice->Release();
        m_pDevice = nullptr;
    }

    if (m_pPool != nullptr)
    {
        m_pPool->Release();
        m_pPool = nullptr;
    }
}

// テクスチャを設定
bool Material::SetTexture(size_t index, TEXTURE_USAGE usage, const std::wstring& path, DirectX::ResourceUploadBatch& batch)
{
    // 範囲内であるかチェック
    if (index >= GetCount())
    {
        return false;
    }

    // 既に登録済かチェック
    if (m_pTexture.find(path) != m_pTexture.end())
    {
        m_Subset[index].TextureHandle[usage] = m_pTexture[path]->GetHandleGPU();
        return true;
    }

    // ファイルパスが存在するかチェック
    std::wstring findPath;
    if (!SearchFilePathW(path.c_str(), findPath))
    {
        // 存在しないダミーテクスチャを設定
        m_Subset[index].TextureHandle[usage] = m_pTexture[L""]->GetHandleGPU();
        return true;
    }

    // ファイル名であることをチェック
    {
        if (PathIsDirectoryW(findPath.c_str()) != FALSE)
        {
            m_Subset[index].TextureHandle[usage] = m_pTexture[L""]->GetHandleGPU();
            return true;
        }
    }

    // インスタンス生成
    auto pTexture = new (std::nothrow) Texture();
    if (pTexture == nullptr)
    {
        return false;
    }

    // 初期化
    if (!pTexture->Init(
        m_pDevice,
        m_pPool,
        findPath.c_str(),
        batch))
    {
        return false;
    }

    // 登録
    m_pTexture[path] = pTexture;
    m_Subset[index].TextureHandle[usage] = pTexture->GetHandleGPU();

    // 正常終了
    return true;
}

// 定数バッファへのポインタを取得
void* Material::GetBufferPtr(size_t index) const
{
    if (index >= GetCount())
    {
        return nullptr;
    }

    return m_Subset[index].pConstantBuffer->GetPtr();
}

// 定数バッファのアドレスを取得
D3D12_GPU_VIRTUAL_ADDRESS Material::GetBufferAddress(size_t index) const
{
    if (index >= GetCount())
    {
        return D3D12_GPU_VIRTUAL_ADDRESS();
    }

    return m_Subset[index].pConstantBuffer->GetAddress();
}

// テクスチャハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE Material::GetTextureHandle(size_t index, TEXTURE_USAGE usage) const
{
    if (index >= GetCount())
    {
        return D3D12_GPU_DESCRIPTOR_HANDLE();
    }

    return m_Subset[index].TextureHandle[usage];
}

// マテリアル数を取得
size_t Material::GetCount() const
{
    return m_Subset.size();
}