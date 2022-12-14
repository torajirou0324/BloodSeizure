#pragma once
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "RootSignature.h"
#include "PipelineState.h"

/// <summary>
/// レンダリングコンテキスト
/// </summary>
class RenderContext
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    RenderContext(){}

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~RenderContext(){}

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
    void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW& vb)
    {
        /*auto vertexBufferDesc = vb.GetView();*/
        m_pCommandList->IASetVertexBuffers(0, 1, &vb);
    }

    /// <summary>
    /// インデックスバッファの設定
    /// </summary>
    /// <param name="ib">インデックスバッファ</param>
    void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW& ib)
    {
        /*auto indexBufferDesc = ib.GetView();*/
        m_pCommandList->IASetIndexBuffer(&ib);
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

    /// <summary>
    /// ビューポートとシザリング矩形を設定
    /// </summary>
    /// <param name="viewport">ビューポート</param>
    void SetViewportAndScissor(D3D12_VIEWPORT& viewport)
    {
        // シザリング矩形設定
        D3D12_RECT rect;
        rect.bottom = static_cast<LONG>(viewport.Height);
        rect.top = 0;
        rect.left = 0;
        rect.right = static_cast<LONG>(viewport.Width);
        SetScissorRect(rect);

        m_pCommandList->RSSetViewports(1, &viewport);
        m_currentViewport = viewport;
    }

    /// <summary>
    /// ビューポートを取得。
    /// </summary>
    /// <returns></returns>
    D3D12_VIEWPORT GetViewport() const
    {
        return m_currentViewport;
    }

    /// <summary>
    /// シザリング矩形を設定
    /// </summary>
    /// <param name="rect"></param>
    void SetScissorRect(D3D12_RECT& rect)
    {
        m_pCommandList->RSSetScissorRects(1, &rect);
    }

    /// <summary>
    /// ルートシグネチャを設定。
    /// </summary>
    void SetRootSignature(ID3D12RootSignature* rootSignature)
    {
        m_pCommandList->SetGraphicsRootSignature(rootSignature);
    }
    //void SetRootSignature(RootSignature& rootSignature)
    //{
    //    auto ptrRootSig = rootSignature.Get();
    //    m_pCommandList->SetGraphicsRootSignature(rootSignature.Get());
    //}

    /// <summary>
    /// パイプラインステートの設定
    /// </summary>
    void SetPipelineState(ID3D12PipelineState* pipelineState)
    {
        m_pCommandList->SetPipelineState(pipelineState);
    }
    //void SetPipelineState(PipelineState& pipelineState)
    //{
    //    auto PSO = pipelineState.Get();
    //    m_pCommandList->SetPipelineState(pipelineState.Get());
    //}

    /// <summary>
    /// インデックス付きプリミティブ描画
    /// </summary>
    /// <param name="indexCount"></param>
    void DrawIndexed(UINT indexCount)
    {
        m_pCommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
    }

    /// <summary>
    /// インスタンシング描画
    /// </summary>
    /// <param name="indexCount"></param>
    /// <param name="numInstance"></param>
    void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
    {
        m_pCommandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
    }

private:
    enum { MAX_DESCRIPTOR_HEAP = 4 };   // ディスクリプタヒープの最大数
    enum { MAX_CONSTANT_BUFFER = 8 };   // 定数バッファの最大数
    enum { MAX_SHADER_RESOURCE = 16 };  // シェーダーリソースの最大数

    D3D12_VIEWPORT m_currentViewport;                                       // 現在のビューポートの設定
    ID3D12GraphicsCommandList* m_pCommandList;                              // コマンドリスト
    ID3D12DescriptorHeap* m_pDescriptorHeaps[MAX_DESCRIPTOR_HEAP];          // ディスクリプタヒープの配列
    //ConstantBuffer* m_pConstantBuffers[MAX_CONSTANT_BUFFER];                // 定数バッファの配列
    //Texture* m_pTextures[MAX_SHADER_RESOURCE];                              // シェーダーリソースの配列
    std::vector<ComPtr<ID3D12Resource>> m_scratchResourceList;              // スクラッチリソース
};