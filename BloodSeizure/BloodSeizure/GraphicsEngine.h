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

    static const int FRAME_BUFFER_COUNT = 2;   // フレームバッファの数
private:
    /// <summary>Direct3Dデバイス</summary>
    ID3D12Device* m_d3dDevice = nullptr;
    /// <summary>コマンドキュー</summary>
    ID3D12CommandQueue* m_commandQueue = nullptr;
    /// <summary>スワップチェイン</summary>
    IDXGISwapChain3* m_swapChain = nullptr;
    /// <summary>コマンドアロケータ</summary>
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    /// <summary>コマンドリスト</summary>
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    /// <summary>パイプラインステート</summary>
    ID3D12PipelineState* m_pipelineState = nullptr;

    /// <summary>現在のバックバッファの番号</summary>
    int m_currentBackBufferIndex = 0;
    /// <summary>フレームバッファのディスクリプタのサイズ</summary>
    UINT m_rtvDescriptorSize = 0;
    /// <summary>深度ステンシルバッファのディスクリプタのサイズ</summary>
    UINT m_dsvDescriptorSize = 0;
    /// <summary>CBR_SRVのディスクリプタのサイズ</summary>
    UINT m_cbvSrvDescriptorSize = 0;
    /// <summary>サンプラーのディスクリプタのサイズ</summary>
    UINT m_samplerDescriptorSize = 0;

    /// <summary></summary>
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    /// <summary></summary>
    ID3D12Resource* m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };
    /// <summary></summary>
    ID3D12DescriptorHeap* m_dsvHeap = nullptr;
    /// <summary></summary>
    ID3D12Resource* m_depthStencilBuffer = nullptr;
    /// <summary></summary>
    D3D12_VIEWPORT m_viewport;
    /// <summary></summary>
    D3D12_RECT m_scissorRect;

    /// <summary></summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;
    /// <summary></summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;
};

/// <summary>グラフィックスエンジンのアドレスを格納するグローバル変数</summary>
extern GraphicsEngine* g_graphicsEngine;