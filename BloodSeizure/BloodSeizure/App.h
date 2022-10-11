#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>

// Linker
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class SceneManager;

class App
{
public:
	App() = delete;
	App(const TCHAR* appName);
	~App();

	void GameLoop();
private:
	void StartApp(const TCHAR* appName);
	void InitWindow(const TCHAR* appName);

	// D3D12 methods
	bool Init3D();
	void Term3D();
	void Render();
	void WaitGpu();
	void Present(UINT32 interval);

	SceneManager* m_sceneManager;		// シーンマネージャークラスのポインタ
	
	static const UINT FrameCount = 2;	// フレームバッファ

	HINSTANCE		m_hInst;				// インスタンスハンドル
	HWND			m_hWnd;					// ウィンドウハンドル
	const UINT		WINDOW_WIDTH = 1280;	// ウィンドウの横幅
	const UINT		WINDOW_HEIGHT = 720;	// ウィンドウの縦幅
	const TCHAR*	ClassName;				// ウィンドウクラス名

	ID3D12Device*				m_pDevice;					// 追加デバイス
	ID3D12CommandQueue*			m_pQueue;					// コマンドキュー
	IDXGISwapChain3*			m_pSwapChain;				// スワップチェイン
	ID3D12Resource*				m_pColorBuffer[FrameCount];	// カラーバッファ
	ID3D12CommandAllocator*		m_pCmdAllocator[FrameCount];// コマンドアローケータ
	ID3D12GraphicsCommandList*	m_pCmdList;					// コマンドリスト
	ID3D12DescriptorHeap*		m_pHeapRTV;					// ディスクリプタヒープ（レンダーターゲットビュー）
	ID3D12Fence*				m_pFence;					// フェンス
	HANDLE						m_FenceEvent;				// フェンスイベント
	UINT16						m_FenceCounter[FrameCount];	// フェンスカウンター
	UINT32						m_FrameIndex;				// フレーム番号
	D3D12_CPU_DESCRIPTOR_HANDLE m_HandleRTV[FrameCount];	// CPUディスクリプタヒープ（レンダーターゲットビュー）
};
