#pragma once

#include "GraphicsEngine.h"

/// <summary>
/// インデックスバッファー
/// </summary>
class IndexBuffer
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    IndexBuffer();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~IndexBuffer();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="size">インデックスバッファのサイズ</param>
    /// <param name="stride">ストライド</param>
    void Init(int size, int stride);

    /// <summary>
    /// インデックスデータをインデックスバッファーにコピー
    /// </summary>
    /// <param name="srcIndices">コピー元のインデックスデータ</param>
    void Copy(uint16_t* srcIndices);

    /// <summary>
    /// インデックスデータをインデックスバッファーにコピー
    /// </summary>
    /// <param name="srcIndices">コピー元のインデックスデータ</param>
    void Copy(uint32_t* srcIndices);
    
    /// <summary>
    /// インデックスバッファーを取得
    /// </summary>
    /// <returns></returns>
    D3D12_INDEX_BUFFER_VIEW GetView() const;

    /// <summary>
    /// インデックスの数を取得
    /// </summary>
    /// <returns></returns>
    int GetCount() const;

    /// <summary>
    /// インデックスバッファのストライドを取得
    /// </summary>
    /// <returns></returns>
    UINT GetStrideInBytes() const;

    /// <summary>
    /// ID3D12Resourceのアドレスを取得
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* GetID3D12ResourceAddress() const;

private:
    ID3D12Resource* m_pIndexBuffer = nullptr;       // インデックスバッファー
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;      // インデックスバッファービュー
    int m_count = 0;                                // インデックスの数
    int m_strideInBytes = 0;                        // ストライド（単位：バイト）
};