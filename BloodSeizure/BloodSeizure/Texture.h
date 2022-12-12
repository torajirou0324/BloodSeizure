#pragma once

#include <ResourceUploadBatch.h>
#include "GraphicsEngine.h"

class Texture
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Texture(){}

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Texture(){}

    /// <summary>
    /// DDSファイルからテクスチャを初期化する
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    void InitDDSFile(const wchar_t* filePath);

    /// <summary>
    /// メモリからテクスチャを初期化する
    /// </summary>
    /// <param name="memory">テクスチャデータが格納されてるメモリの先頭アドレス</param>
    /// <param name="size">テクスチャのサイズ</param>
    void InitMemory(const char* memory, unsigned int size);

    /// <summary>
    /// D3Dリソースからテクスチャを初期化する
    /// </summary>
    /// <param name="texture">D3Dリソース</param>
    void InitD3DResource(ID3D12Resource* texture);

    /// <summary>
    /// シェーダーリソースビューを登録(SRV)
    /// </summary>
    /// <param name="descriptorHandle"></param>
    /// <param name="bufferNo"></param>
    void SetShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

    /// <summary>
    /// テクスチャのリソースを取得
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* Get()
    {
        return m_pTexture;
    }

    /// <summary>
    /// テクスチャの幅を取得
    /// </summary>
    /// <returns></returns>
    int GetWidth() const
    {
        return static_cast<int>(m_textureDesc.Width);
    }

    /// <summary>
    /// テクスチャの高さを取得
    /// </summary>
    /// <returns></returns>
    int GetHeight() const
    {
        return static_cast<int>(m_textureDesc.Height);
    }

private:
    /// <summary>
    /// DDSファイルからテクスチャをロード
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    void LoadTextureFromDDSFile(const wchar_t* filePath);

    /// <summary>
    /// メモリからテクスチャをロード
    /// </summary>
    /// <param name="memory">テクスチャデータが格納されてるメモリの先頭アドレス</param>
    /// <param name="size">テクスチャサイズ</param>
    void LoadTextureFromMemory(const char* memory, unsigned int size);

    /// <summary>
    /// シェーダーリソースの設定を取得
    /// </summary>
    /// <param name="isCube">キューブマップ</param>
    /// <returns></returns>
    D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool isCube);

    ID3D12Resource* m_pTexture = nullptr;
    D3D12_RESOURCE_DESC m_textureDesc;
};