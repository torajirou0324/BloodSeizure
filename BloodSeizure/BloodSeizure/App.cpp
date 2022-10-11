#include <iostream>
#include "App.h"
#include "SceneManager.h"

template<typename T>
void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

void App::InitWindow(const TCHAR* appName)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
	{
		return;
	}

	// ウィンドウの設定
	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.hIcon			= LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground	= GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= appName;
	wc.hIconSm			= LoadIcon(hInst, IDI_APPLICATION);

	// ウィンドウクラスの登録
	RegisterClassEx(&wc);

	// インスタンスハンドルの設定
	m_hInst = hInst;

	// ウィンドウサイズの設定
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);

	// ウィンドウサイズを調整
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	// ウィンドウの生成
	m_hWnd = CreateWindowEx(
		0,
		appName,
		appName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		m_hInst,
		nullptr
	);

	// ウィンドウを表示
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// ウィンドウにフォーカスする
	SetFocus(m_hWnd);
}

bool App::Init3D()
{
	// デバイスの生成
	auto hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice)
	);
	if (FAILED(hr))
	{
		return false;
	}

	// コマンドキューの生成
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT;		// コマンドリストタイプ
		desc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// 優先度
		desc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;		// 特性
		desc.NodeMask	= 0;									// 使用するGPUが1つなので0
	}

	// スワップチェイン
	{
		// DXGIファクトリーの生成
		IDXGIFactory4* pFactory = nullptr;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
		{
			return false;
		}

		// スワップチェインの設定
		DXGI_SWAP_CHAIN_DESC desc = {};
		// BufferDesc(バックバッファーの表示モードをDXGI_MODE_DESC構造体で設定)
		desc.BufferDesc.Width					= WINDOW_WIDTH;							// 解像度の横幅
		desc.BufferDesc.Height					= WINDOW_HEIGHT;						// 解像度の縦幅
		desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// 走査線描画モード指定
		desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		// 拡大縮小の設定を指定
		desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			// 表示形式のピクセルフォーマットを指定
		// RefreshRateは(DXGI_RATIONAL構造体に定義されている分母と分子を決める変数の値を設定して指定)60/1 Hz = 60Hz
		desc.BufferDesc.RefreshRate.Numerator	= 60;									// 分子
		desc.BufferDesc.RefreshRate.Denominator	= 1;									// 分母
		// マルチサンプリングについての設定をDXGI_SAMPLE_DESC構造体で指定
		desc.SampleDesc.Count					= 1;									// ピクセル単位のマルチサンプリングの数
		desc.SampleDesc.Quality					= 0;									// 画像の品質レベルを指定
		desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;		// バックバッファの使用方法を指定
		desc.BufferCount						= FrameCount;							// バックバッファの数を指定（ダブルバッファリングなので2）
		desc.OutputWindow						= m_hWnd;								// 出力するウインドウへのハンドルを指定
		desc.Windowed							= TRUE;
		desc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;		// 描画が終わった後にバックバッファの内容を破棄
		desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;// 動作オプションを指定

		// スワップチェインの生成
		IDXGISwapChain* pSwapChain = nullptr;
		hr = pFactory->CreateSwapChain(m_pQueue, &desc, &pSwapChain);
		if(FAILED(hr))
		{
			SafeRelease(pFactory);
			return false;
		}

		// IDXGISwapChain3を取得
		hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));	// バックバッファ番号の取得メソッドを取得
		if (FAILED(hr))
		{
			SafeRelease(pFactory);
			SafeRelease(pSwapChain);
			return false;
		}

		// バックバッファ番号を取得
		m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		// 不要になったので解放
		SafeRelease(pFactory);
		SafeRelease(pSwapChain);
	}

	// コマンドアローケータの生成（コマンドリストが使用するメモリを割り当てするためにコマンドアローケータを作成）
	{
		// ダブルバッファリングを使用するため2フレーム分のコマンドアローケータを生成
		for (auto i = 0; i < FrameCount; ++i)
		{
			hr = m_pDevice->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&m_pCmdAllocator[i])
			);
			if (FAILED(hr))
			{
				return false;
			}
		}
	}

	// コマンドリストの生成
	{
		hr = m_pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pCmdAllocator[m_FrameIndex],
			nullptr,
			IID_PPV_ARGS(&m_pCmdList)
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	// レンダーターゲットビューの生成（描画先）
	{
		// ディスクリプタヒープの設定
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors				= FrameCount;
		desc.Type						= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags						= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask					= 0;

		// ディスクリプタヒープを生成
		hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pHeapRTV));
		if (FAILED(hr))
		{
			return false;
		}

		auto handle = m_pHeapRTV->GetCPUDescriptorHandleForHeapStart();
		auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (auto i = 0; i < FrameCount; ++i)
		{
			hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pColorBuffer[i]));
			if (FAILED(hr))
			{
				return false;
			}

			D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
			viewDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			viewDesc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipSlice		= 0;
			viewDesc.Texture2D.PlaneSlice	= 0;

			// レンダーターゲットビューの生成
			m_pDevice->CreateRenderTargetView(m_pColorBuffer[i], &viewDesc, handle);

			m_HandleRTV[i] = handle;
			handle.ptr += incrementSize;
		}
	}

	// フェンス
	{
		// フェンスのカウンターをリセット
		for (auto i = 0; i < FrameCount; ++i)
		{
			m_FenceCounter[i] = 0;
		}

		// フェンスの生成
		hr = m_pDevice->CreateFence(
			m_FenceCounter[m_FrameIndex],
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_pFence)
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	// コマンドリストを閉じる
	m_pCmdList->Close();

	return true;
}

void App::Term3D()
{
}

void App::Render()
{
}

void App::WaitGpu()
{
}

void App::Present(UINT32 interval)
{
}


App::App(const TCHAR* appName)
	: m_sceneManager(nullptr)
	, ClassName(appName)
{
	StartApp(appName);
}

App::~App()
{
	// シーン管理クラスを解放
	if (m_sceneManager != nullptr)
	{
		delete m_sceneManager;
	}

	// ウィンドウの登録を解除
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd  = nullptr;
}

void App::StartApp(const TCHAR* appName)
{
	// ウィンドウ生成
	InitWindow(appName);

	// シーン管理クラス生成
	if (m_sceneManager == nullptr)
	{
		m_sceneManager = new SceneManager();
		m_sceneManager->SetNowScene(TAG_SCENE::TITLE);
	}
}

void App::GameLoop()
{
	m_sceneManager->GameLoop();
}
