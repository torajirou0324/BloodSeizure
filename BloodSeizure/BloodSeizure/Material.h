#pragma once

#include <ResourceUploadBatch.h>
#include <map>
#include "DescriptorPool.h"
#include "Texture.h"
#include "ConstantBuffer.h"

class Material
{
public:
    // テクスチャの設定
    enum TEXTURE_USAGE
    {
        TEXTURE_USAGE_DIFFUSE = 0,  // ディフューズマップとして利用
        TEXTURE_USAGE_SPECULAR,     // スペキュラマップとして利用
        TEXTURE_USAGE_SHININESS,    // シャイネスマップとして利用
        TEXTURE_USAGE_NORMAL,       // 法線マップとして利用

        TEXTURE_USAGE_COUNT
    };

    Material();
    ~Material();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, size_t bufferSize, size_t count);

    // 解放処理
    void Term();

    // テクスチャ設定
    bool SetTexture(size_t index, TEXTURE_USAGE usage, const std::wstring& path, DirectX::ResourceUploadBatch& batch);

    // 定数バッファのポインタを取得
    void* GetBufferPtr(size_t index) const;

    // 定数バッファのポインタを取得
    template<typename T>
    T* GetBufferPtr(size_t index) const
    {
        return reinterpret_cast<T*>(GetBufferPtr(index));
    }

    // 定数バッファのGPU仮想アドレスを取得
    D3D12_GPU_VIRTUAL_ADDRESS GetBufferAddress(size_t index) const;

    // テクスチャハンドルを取得
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(size_t index, TEXTURE_USAGE usage) const;

    // マテリアル数を取得
    size_t GetCount() const;

private:
    struct Subset
    {
        ConstantBuffer*             pConstantBuffer;                    // 定数バッファ
        D3D12_GPU_DESCRIPTOR_HANDLE TextureHandle[TEXTURE_USAGE_COUNT]; // テクスチャハンドル
    };

    std::map<std::wstring, Texture*>    m_pTexture;     // テクスチャセット
    std::vector<Subset>                 m_Subset;       // サブセット
    ID3D12Device*                       m_pDevice;      // デバイス
    DescriptorPool*                     m_pPool;        // ディスクリプタヒープ(CBV_UAV_SRV)

    Material(const Material&) = delete;
    void operator = (const Material&) = delete;
};

constexpr auto TU_DIFFUSE   = Material::TEXTURE_USAGE_DIFFUSE;
constexpr auto TU_SPECULAR  = Material::TEXTURE_USAGE_SPECULAR;
constexpr auto TU_SHININESS = Material::TEXTURE_USAGE_SHININESS;
constexpr auto TU_NORMAL    = Material::TEXTURE_USAGE_NORMAL;