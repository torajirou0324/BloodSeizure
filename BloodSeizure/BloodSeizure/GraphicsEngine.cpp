#include "GraphicsEngine.h"
#include "ComPtr.h"

GraphicsEngine* g_graphicsEngine = nullptr;

bool GraphicsEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	m_frameBufferWidth  = frameBufferWidth;
	m_frameBufferHeight = frameBufferHeight;

	// �f�o�C�X�̐���
	auto hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&m_pDevice)
	);
	if (FAILED(hr))
	{
		return false;
	}

	// �R�}���h�L���[�̍쐬
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

	// �X���b�v�`�F�C���̍쐬
	{
		// DXGI�t�@�N�g���[�̐���
		ComPtr<IDXGIFactory4> pFactory;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
		{
			return false;
		}

		// �X���b�v�`�F�C���̐ݒ�
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

		// �X���b�v�`�F�C���̐���
		ComPtr<IDXGISwapChain> pSwapChain;
		hr = pFactory->CreateSwapChain(m_pCommandQueue.Get(), &desc, pSwapChain.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}

		// IDXGISwapChain3�̎擾
		hr = pSwapChain.As(&m_pSwapChain);
		if (FAILED(hr))
		{
			return false;
		}

		// �o�b�N�o�b�t�@�ԍ����擾
		m_currentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		// �s�v�Ȃ̂ŉ��
		pFactory.Reset();
		pSwapChain.Reset();
	}

	// �t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v�̐���
	{
		// RTV�p�̃f�B�X�N���v�^�q�[�v�𐶐�
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

		// �f�B�X�N���v�^�̃T�C�Y���擾
		m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// DSV�p�̃f�B�X�N���v�^�q�[�v�𐶐�
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		
		hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDsvHeap));
		if (FAILED(hr))
		{
			return false;
		}

		// �f�B�X�N���v�^�̃T�C�Y���擾
		m_dsvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g�r���[�𐶐�
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();

		DXGI_SWAP_CHAIN_DESC desc;
		m_pSwapChain->GetDesc(&desc);

		D3D12_RENDER_TARGET_VIEW_DESC Viewdesc = {};
		Viewdesc.ViewDimension			= D3D12_RTV_DIMENSION_TEXTURE2D;
		Viewdesc.Format					= desc.BufferDesc.Format;
		Viewdesc.Texture2D.MipSlice		= 0;
		Viewdesc.Texture2D.PlaneSlice	= 0;

		// �t�����g�o�b�t�@���o�b�N�o�b�t�@�p��RTV�𐶐�
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

	// �t���[���o�b�t�@�p�̃f�v�X�X�e���V���r���[�𐶐�
	{
		// �v���p�e�B�ݒ�
		D3D12_HEAP_PROPERTIES prop = {};
		prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask		= 1;
		prop.VisibleNodeMask		= 1;

		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment			= 0;
		desc.Width				= UINT64(m_frameBufferWidth);
		desc.Height				= m_frameBufferHeight;
		desc.DepthOrArraySize	= 1;
		desc.MipLevels			= 1;
		desc.Format				= DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

		// �N���A�ݒ�
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format				= DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth	= 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		// ���\�[�X�𐶐�
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

		// �[�x�X�e���V���r���[��ݒ�
		D3D12_DEPTH_STENCIL_VIEW_DESC Viewdesc = {};
		Viewdesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D;
		Viewdesc.Format				= DXGI_FORMAT_D32_FLOAT;
		Viewdesc.Texture2D.MipSlice = 0;
		Viewdesc.Flags				= D3D12_DSV_FLAG_NONE;

		// �f�B�X�N���v�^�𐶐�
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

		// �[�x�X�e���V���r���[�𐶐�
		m_pDevice->CreateDepthStencilView(
			m_pDepthStencilBuffer, &Viewdesc, dsvHandle
		);
	}

	// �R�}���h�A���P�[�^�̐���
	for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		auto hr = m_pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator[i])
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	// �R�}���h���X�g�̐���
	{
		auto hr = m_pDevice->CreateCommandList(
			1,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pCommandAllocator[0].Get(),
			nullptr,
			IID_PPV_ARGS(m_pCommandList.GetAddressOf())
		);
		if (FAILED(hr))
		{
			return false;
		}

		// �R�}���h���X�g���J����Ă����Ԃō쐬����邽�߁A��x����
		m_pCommandList->Close();
	}

	// �t�F���X�𐶐�
	{
		// �C�x���g�𐶐�(�������s���Ƃ��̃C�x���g�n���h��)
		m_FenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		if (m_FenceEvent == nullptr)
		{
			return false;
		}

		// �t�F���X�𐶐�
		auto hr = m_pDevice->CreateFence(
			0,
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_pFence)
		);
		if (FAILED(hr))
		{
			return false;
		}

		// �J�E���^��ݒ�
		m_fenceValue = 1;
	}

	m_renderContext.Init(m_pCommandList.Get());

	// �r���[�|�[�g
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = float(frameBufferWidth);
	m_viewport.Height = float(frameBufferHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	// �V�U�[��`
	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = frameBufferWidth;
	m_scissorRect.bottom = frameBufferHeight;

	// CBV_SRV_UAV�̃f�B�X�N���v�^�T�C�Y���擾
	m_cbvSrvUavDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// Sampler�̃f�B�X�N���v�^�T�C�Y���擾
	m_samplerDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);



	return true;
}

void GraphicsEngine::BeginRender()
{
	// �R�}���h���X�g�̋L�^���J�n
	m_pCommandAllocator[m_frameIndex]->Reset();
	m_pCommandList->Reset(m_pCommandAllocator[m_frameIndex].Get(), m_pPipelineState.Get());
	m_frameIndex = (m_frameIndex + 1) % uint32_t(FRAME_BUFFER_COUNT);

	// �����_�[�^�[�Q�b�g�r���[���̃f�B�X�N���v�^�q�[�v�̃A�h���X���擾
	m_currentFrameBufferRTVHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
	m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;

	// �[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v�̊J�n�A�h���X���擾
	m_currentFrameBufferDSVHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

	// �`�����ݗp���\�[�X�o���A�ݒ�i�����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂j
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pRenderTargets[m_frameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	// �o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ�\�ɂȂ�܂ő҂�
	m_pCommandList->ResourceBarrier(1, &barrier);

	// �����_�[�^�[�Q�b�g��ݒ�
	m_pCommandList->OMSetRenderTargets(1, &m_currentFrameBufferRTVHandle, FALSE, &m_currentFrameBufferDSVHandle);
	
	// �N���A�J���[
	float clearColor[] = { 0.25f,0.25f,0.25f,1.0f };

	// �����_�[�^�[�Q�b�g�r���[�̃N���A
	m_pCommandList->ClearRenderTargetView(m_currentFrameBufferRTVHandle, clearColor, 0, nullptr);

	// �f�v�X�X�e���V���r���[�̃N���A
	m_pCommandList->ClearDepthStencilView(
		m_currentFrameBufferDSVHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);
}

void GraphicsEngine::EndRender()
{
	// �����_�[�^�[�Q�b�g�r���[�ւ̕`�����݊����҂�
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pRenderTargets[m_frameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pCommandList->ResourceBarrier(1, &barrier);

	// �R�}���h���X�g�̋L�^���I��
	m_pCommandList->Close();

	ID3D12CommandList* pLists[] = { m_pCommandList.Get() };

	m_pCommandQueue->ExecuteCommandLists(1, pLists);

	// ��ʂɕ\��
	Present(1);
}

GraphicsEngine::~GraphicsEngine()
{
	WaitDraw();

	// �������
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
	for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		if (m_pCommandAllocator[i])
		{
			m_pCommandAllocator[i]->Release();
		}
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

	CloseHandle(m_FenceEvent);
}

void GraphicsEngine::Present(uint32_t interval)
{
	// ��ʂɕ\��
	m_pSwapChain->Present(interval, 0);

	// �`�抮���҂�
	WaitDraw();

	// �t���[���ԍ����X�V
	m_frameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
}

void GraphicsEngine::WaitDraw()
{
	// �M���ƃt�F���X�l���C���N�������g����
	const UINT64 fence = m_fenceValue;
	m_pCommandQueue->Signal(m_pFence, fence);
	m_fenceValue++;

	// �O�̃t���[�����I������܂ő҂��܂��B
	if (m_pFence->GetCompletedValue() < fence)
	{
		m_pFence->SetEventOnCompletion(fence, m_FenceEvent);
		WaitForSingleObject(m_FenceEvent, INFINITE);
	}
}