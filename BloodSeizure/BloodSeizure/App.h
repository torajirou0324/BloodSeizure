#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "ComPtr.h"

#pragma comment( lib , "d3d12.lib" )
#pragma comment( lib , "dxgi.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "d3dcompiler.lib" )

class App
{
public:
	App(uint32_t width, uint32_t height);
	~App();

	void Run();

protected:

	enum POOL_TYPE
	{
		POOL_TYPE_RES = 0,	// CBV / SRV / UAV
		POOL_TYPE_SMP = 1,	// Sampler
		POOL_TYPE_RTV = 2,	// RTV
		POOL_TYPE_DSV = 3,	// DSV
		POOL_COUNT    = 4,
	};
	// private変数
	static const uint32_t FrameCount = 2;	// フレームバッファ数

	HINSTANCE m_hInst;		// インスタンスハンドル
	HWND m_hWnd;			// ウィンドウハンドル
	uint32_t m_Width;		// ウィンドウの横幅
	uint32_t m_Height;		// ウィンドウの縦幅

	ComPtr<ID3D12Device>				m_pDevice;						// デバイス
	ComPtr<ID3D12CommandQueue>			m_pQueue;						// コマンドキュー
	ComPtr<IDXGISwapChain3>				m_pSwapChain;					// スワップチェイン
	ComPtr<ID3D12GraphicsCommandList>	m_pCmdList;						// コマンドリスト
	uint32_t							m_FrameIndex;					// フレーム番号
	D3D12_VIEWPORT						m_Viewport;						// ビューポート
	D3D12_RECT							m_Scissor;						// シザー矩形

private:
	// private関数
	bool InitApp();
	void TermApp();
	bool InitWnd();
	void TermWnd();
	bool InitD3D();
	void TermD3D();
	void MainLoop();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};