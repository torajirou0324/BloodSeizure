#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "ComPtr.h"

#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

/// <summary>
/// DirectX12のグラフィックエンジン
/// </summary>
class GraphicsEngine
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    GraphicsEngine(){}

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GraphicsEngine();

    /// <summary>
    /// DirectX12の機能を初期化
    /// </summary>
    /// <param name="hwnd">ウィンドウハンドル</param>
    /// <param name="frameBufferWidth">ウィンドウ画面の横幅</param>
    /// <param name="frameBufferHeight">ウィンドウ画面の縦幅</param>
    /// <returns>falseが返ってきたら作成に失敗 </returns>
    bool Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

    /// <summary>
    /// レンダリング開始
    /// </summary>
    /// 毎フレームのレンダリング開始時に呼ぶもの
    void BeginRender();

    /// <summary>
    /// レンダリング終了
    /// </summary>
    /// 毎フレームのレンダリング終了時に呼ぶもの
    void EndRender();

    GraphicsEngine(const GraphicsEngine&) = delete;             // コピーコンストラクタをdelete指定
    GraphicsEngine& operator=(const GraphicsEngine&) = delete;  // コピー代入演算子もdelete指定
    GraphicsEngine(GraphicsEngine&&) = delete;                  // ムーブコンストラクタをdelete指定
    GraphicsEngine& operator=(GraphicsEngine&&) = delete;       // ムーブ代入演算子もdelete指定

    /// <summary>
    /// DirectX12のグラフィックエンジンを生成
    /// </summary>
    static void Instance()
    {
        if (m_pGraphicsEngine != nullptr)
        {
            m_pGraphicsEngine = new GraphicsEngine;
            g_graphicsEngine = m_pGraphicsEngine;
        }
    }

    /// <summary>
    /// DirectX12のグラフィックエンジンを解放
    /// </summary>
    static void Terminate()
    {
        if (m_pGraphicsEngine)
        {
            delete m_pGraphicsEngine;
            m_pGraphicsEngine = nullptr;
        }
    }

private:
    /// <summary>
    /// 描画の完了待ち
    /// </summary>
    void WaitDraw();

public:
    /// <summary>
    /// フレームバッファの数
    /// </summary>
    static const int FRAME_BUFFER_COUNT = 2;

private:
    /// <summary>グラフィックスエンジンのアドレスを格納するメンバ変数</summary>
    static GraphicsEngine* m_pGraphicsEngine;

    /// <summary>Direct3Dデバイス</summary>
    ComPtr<ID3D12Device> m_pDevice = nullptr;
    /// <summary>コマンドキュー</summary>
    ComPtr<ID3D12CommandQueue> m_pCommandQueue = nullptr;
    /// <summary>スワップチェイン</summary>
    ComPtr<IDXGISwapChain3> m_pSwapChain = nullptr;
    /// <summary>コマンドアロケータ</summary>
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator = nullptr;
    /// <summary>コマンドリスト</summary>
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList = nullptr;
    /// <summary>パイプラインステート</summary>
    ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;

    /// <summary>現在のバックバッファの番号</summary>
    int m_currentBackBufferIndex = 0;
    /// <summary>フレームバッファのディスクリプタのサイズ</summary>
    UINT m_rtvDescriptorSize = 0;
    /// <summary>深度ステンシルバッファのディスクリプタのサイズ</summary>
    UINT m_dsvDescriptorSize = 0;
    /// <summary>CBV_SRV_UAVのディスクリプタのサイズ</summary>
    UINT m_cbvSrvUavDescriptorSize = 0;
    /// <summary>サンプラーのディスクリプタのサイズ</summary>
    UINT m_samplerDescriptorSize = 0;

    /// <summary>レンダーターゲットビューのディスクリプタヒープ</summary>
    ID3D12DescriptorHeap* m_pRtvHeap = nullptr;
    /// <summary>フレームバッファ用のレンダーターゲット</summary>
    ID3D12Resource* m_pRenderTargets[FRAME_BUFFER_COUNT] = { nullptr };
    /// <summary>深度ステンシルビューのディスクリプタヒープ</summary>
    ID3D12DescriptorHeap* m_pDsvHeap = nullptr;
    /// <summary>深度ステンシルバッファ</summary>
    ID3D12Resource* m_pDepthStencilBuffer = nullptr;
    /// <summary>ビューポート</summary>
    D3D12_VIEWPORT m_pViewport;
    /// <summary>シザリング矩形</summary>
    D3D12_RECT m_pScissorRect;

    /// <summary>現在書き込み中のフレームバッファのレンダリングターゲットビューのハンドル</summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;
    /// <summary>現在書き込み中のフレームバッファの深度ステンシルビューのハンドル</summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;

    /// <summary>
    /// GPUとの同期で使用する変数
    /// </summary>
    UINT m_frameIndex = 0;
    HANDLE m_pFenceEvent = nullptr;
    ID3D12Fence* m_pFence = nullptr;
    UINT64 m_fenceValue = 0;
    UINT m_frameBufferWidth = 0;
    UINT m_frameBufferHeight = 0;
};

/// <summary>グラフィックスエンジンのアドレスを格納するグローバル変数</summary>
extern GraphicsEngine* g_graphicsEngine;