#pragma once

#include "GraphicsEngine.h"

/// <summary>
/// 頂点バッファ
/// </summary>
class VertexBuffer
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    VertexBuffer();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~VertexBuffer();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="size">頂点バッファのサイズ</param>
    /// <param name="stride">頂点一個のサイズ</param>
    void Init(void* size, int stride);

    /// <summary>
    /// 頂点データを頂点バッファにコピー
    /// </summary>
    /// <param name="srcVertices">コピー元の頂点データ</param>
    //void Copy(void* srcVertices);

    /// <summary>
    /// ID3D12Resourceのアドレスを取得
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* GetID3DResourceAddress() const;

    /// <summary>
    /// 頂点バッファビューを取得
    /// </summary>
    /// <returns></returns>
    D3D12_VERTEX_BUFFER_VIEW GetView() const;

private:
    ID3D12Resource* m_pVertexBuffer = nullptr;      // 頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;    // 頂点バッファビュー
};