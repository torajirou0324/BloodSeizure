#include "GraphicsEngine.h"
#include "ComPtr.h"

GraphicsEngine* GraphicsEngine::m_pGraphicsEngine = nullptr;
GraphicsEngine* g_graphicsEngine = nullptr;

bool GraphicsEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_frameBufferWidth  = frameBufferWidth;
	m_frameBufferHeight = frameBufferHeight;

	// デバイスの生成
	auto hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&m_pDevice)
	);
	if (FAILED(hr))
	{
		return false;
	}

	// コマンドキューの作成
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type		 = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority	 = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags		 = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask	 = 0;

		hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_pCommandQueue));
		if (FAILED(hr))
		{
			return false;
		}
	}

	// スワップチェインの作成
	{
		// DXGIファクトリーの生成
		ComPtr<IDXGIFactory4> pFactory;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
		{
			return false;
		}

		// スワップチェインの設定
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferDesc.Width					 = m_frameBufferWidth;
		desc.BufferDesc.Height					 = m_frameBufferHeight;
		desc.BufferDesc.RefreshRate.Numerator	 = 60;
		desc.BufferDesc.RefreshRate.Denominator	 = 1;
		desc.BufferDesc.ScanlineOrdering		 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling					 = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count					 = 1;
		desc.SampleDesc.Quality					 = 0;
		desc.BufferUsage						 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount						 = FRAME_BUFFER_COUNT;
		desc.OutputWindow						 = hwnd;
		desc.Windowed							 = TRUE;
		desc.SwapEffect							 = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags								 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// スワップチェインの生成
		ComPtr<IDXGISwapChain> pSwapChain;
		hr = pFactory->CreateSwapChain(m_pCommandQueue.Get(), &desc, pSwapChain.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}

		// IDXGISwapChain3の取得
		hr = pSwapChain.As(&m_pSwapChain);
		if (FAILED(hr))
		{
			return false;
		}

		// バックバッファ番号を取得
		m_currentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		// 不要なので解放
		pFactory.Reset();
		pSwapChain.Reset();
	}

	// フレームバッファ用のディスクリプタヒープの生成
	{
		// RTV用のディスクリプタヒープを生成
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NodeMask = 0;
		desc.NumDescriptors = FRAME_BUFFER_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		auto hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pRtvHeap));
		if (FAILED(hr))
		{
			return false;
		}

		// ディスクリプタのサイズを取得
		m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// DSV用のディスクリプタヒープを生成
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		
		hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDsvHeap));
		if (FAILED(hr))
		{
			return false;
		}

		// ディスクリプタのサイズを取得
		m_dsvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	// フレームバッファ用のレンダーターゲットビューを生成
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();

		DXGI_SWAP_CHAIN_DESC desc;
		m_pSwapChain->GetDesc(&desc);

		D3D12_RENDER_TARGET_VIEW_DESC Viewdesc = {};
		Viewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		Viewdesc.Format = desc.BufferDesc.Format;
		Viewdesc.Texture2D.MipSlice = 0;
		Viewdesc.Texture2D.PlaneSlice = 0;

		// フロントバッファをバックバッファ用のRTVを生成
		for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRenderTargets[i]));
			m_pDevice->CreateRenderTargetView(
				m_pRenderTargets[i],
				&Viewdesc,
				rtvHandle
			);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
	}

	// フレームバッファ用のデプスステンシルビューを生成
	{
		// プロパティ設定
		D3D12_HEAP_PROPERTIES prop = {};
		prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 1;
		prop.VisibleNodeMask = 1;

		// リソース設定
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = UINT64(m_frameBufferWidth);
		desc.Height = m_frameBufferHeight;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// クリア設定
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		// リソースを生成
		auto hr = m_pDevice->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&m_pDepthStencilBuffer)
		);
		if (FAILED(hr))
		{
			return false;
		}

		// 深度ステンシルビューを設定
		D3D12_DEPTH_STENCIL_VIEW_DESC Viewdesc = {};
		Viewdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		Viewdesc.Format = DXGI_FORMAT_D32_FLOAT;
		Viewdesc.Texture2D.MipSlice = 0;
		Viewdesc.Flags = D3D12_DSV_FLAG_NONE;

		// 深度ステンシルビューを生成
	}

	return true;
}

void GraphicsEngine::BeginRender()
{
}

void GraphicsEngine::EndRender()
{
}

GraphicsEngine::~GraphicsEngine()
{
	WaitDraw();

	// 解放処理
	if (m_pCommandQueue)
	{
		m_pCommandQueue->Release();
	}
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
	}
	if (m_pRtvHeap)
	{
		m_pRtvHeap->Release();
	}
	if (m_pDsvHeap)
	{
		m_pDsvHeap->Release();
	}
	if (m_pCommandAllocator)
	{
		m_pCommandAllocator->Release();
	}
	if (m_pCommandList)
	{
		m_pCommandList->Release();
	}
	if (m_pPipelineState)
	{
		m_pPipelineState->Release();
	}
	for (auto& rt : m_pRenderTargets)
	{
		if (rt)
		{
			rt->Release();
		}
	}
	if (m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
	}
	if (m_pFence)
	{
		m_pFence->Release();
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
	}

	CloseHandle(m_pFenceEvent);
}

void GraphicsEngine::WaitDraw()
{
	// 信号とフェンス値をインクリメントする
	const UINT64 fence = m_fenceValue;
	m_pCommandQueue->Signal(m_pFence, fence);
	m_fenceValue++;

	// 前のフレームが終了するまで待ちます。
	if (m_pFence->GetCompletedValue() < fence)
	{
		m_pFence->SetEventOnCompletion(fence, m_pFenceEvent);
		WaitForSingleObject(m_pFenceEvent, INFINITE);
	}
}