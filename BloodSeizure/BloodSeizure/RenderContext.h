#pragma once
#include <vector>
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"

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
    void SetRootSignature(RootSignature& rootSignature)
    {
        m_pCommandList->SetGraphicsRootSignature(rootSignature.Get());
    }
    void SetComputeRootSignature(ID3D12RootSignature* rootSignature)
    {
        m_pCommandList->SetComputeRootSignature(rootSignature);
    }
    void SetComputeRootSignature(RootSignature& rootSignature)
    {
        m_pCommandList->SetComputeRootSignature(rootSignature.Get());
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