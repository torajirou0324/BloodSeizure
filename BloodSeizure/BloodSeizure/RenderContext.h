#pragma once
#include <vector>
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Texture;

/// <summary>
/// レンダリングコンテキスト
/// </summary>
class RenderContext
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    RenderContext();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~RenderContext();

    /// <summary>
    /// 初期化。
    /// </summary>
    /// <param name="commandList">コマンドリスト</param>
    void Init(ID3D12GraphicsCommandList* commandList)
    {
        m_pCommandList = commandList;
    }

    /// <summary>
    /// 頂点バッファの設定
    /// </summary>
    /// <param name="vb">頂点バッファ</param>
    void SetVertexBuffer(VertexBuffer& vb)
    {
        m_pCommandList->IASetVertexBuffers(0, 1, &vb.GetView());
    }

    /// <summary>
    /// インデックスバッファの設定
    /// </summary>
    /// <param name="ib">インデックスバッファ</param>
    void SetIndexBuffer(IndexBuffer& ib)
    {
        m_pCommandList->IASetIndexBuffer(&ib.GetView());
    }

    /// <summary>
    /// プリミティブトポロジーを設定
    /// </summary>
    /// <param name="topology">プリミティブトポロジーのタイプ</param>
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
    {
        m_pCommandList->IASetPrimitiveTopology(topology);
    }

    /// <summary>
    /// コマンドリストを設定
    /// </summary>
    /// <param name="commandList">コマンドリスト</param>
    void SetCommandList(ID3D12GraphicsCommandList* commandList)
    {
        m_pCommandList = commandList;
    }

    void SetPipelineState()
    {

    }

private:
    enum { MAX_DESCRIPTOR_HEAP = 4 };   // ディスクリプタヒープの最大数
    enum { MAX_CONSTANT_BUFFER = 8 };   // 定数バッファの最大数
    enum { MAX_SHADER_RESOURCE = 16 };  // シェーダーリソースの最大数

    D3D12_VIEWPORT m_currentViewport;                                       // 現在のビューポートの設定
    ID3D12GraphicsCommandList* m_pCommandList;                              // コマンドリスト
    ID3D12DescriptorHeap* m_pDescriptorHeaps[MAX_DESCRIPTOR_HEAP];          // ディスクリプタヒープの配列
    ConstantBuffer* m_pConstantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };  // 定数バッファの配列
    Texture* m_pTextures[MAX_SHADER_RESOURCE] = { nullptr };                // シェーダーリソースの配列
    std::vector<ComPtr<ID3D12Resource>> m_scratchResourceList;              // スクラッチリソース
};