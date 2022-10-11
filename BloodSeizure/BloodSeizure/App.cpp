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

	// �E�B���h�E�̐ݒ�
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

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �C���X�^���X�n���h���̐ݒ�
	m_hInst = hInst;

	// �E�B���h�E�T�C�Y�̐ݒ�
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);

	// �E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	// �E�B���h�E�̐���
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

	// �E�B���h�E��\��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// �E�B���h�E�Ƀt�H�[�J�X����
	SetFocus(m_hWnd);
}

bool App::Init3D()
{
	// �f�o�C�X�̐���
	auto hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice)
	);
	if (FAILED(hr))
	{
		return false;
	}

	// �R�}���h�L���[�̐���
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT;		// �R�}���h���X�g�^�C�v
		desc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// �D��x
		desc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;		// ����
		desc.NodeMask	= 0;									// �g�p����GPU��1�Ȃ̂�0
	}

	// �X���b�v�`�F�C��
	{
		// DXGI�t�@�N�g���[�̐���
		IDXGIFactory4* pFactory = nullptr;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
		{
			return false;
		}

		// �X���b�v�`�F�C���̐ݒ�
		DXGI_SWAP_CHAIN_DESC desc = {};
		// BufferDesc(�o�b�N�o�b�t�@�[�̕\�����[�h��DXGI_MODE_DESC�\���̂Őݒ�)
		desc.BufferDesc.Width					= WINDOW_WIDTH;							// �𑜓x�̉���
		desc.BufferDesc.Height					= WINDOW_HEIGHT;						// �𑜓x�̏c��
		desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// �������`�惂�[�h�w��
		desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		// �g��k���̐ݒ���w��
		desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			// �\���`���̃s�N�Z���t�H�[�}�b�g���w��
		// RefreshRate��(DXGI_RATIONAL�\���̂ɒ�`����Ă��镪��ƕ��q�����߂�ϐ��̒l��ݒ肵�Ďw��)60/1 Hz = 60Hz
		desc.BufferDesc.RefreshRate.Numerator	= 60;									// ���q
		desc.BufferDesc.RefreshRate.Denominator	= 1;									// ����
		// �}���`�T���v�����O�ɂ��Ă̐ݒ��DXGI_SAMPLE_DESC�\���̂Ŏw��
		desc.SampleDesc.Count					= 1;									// �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�
		desc.SampleDesc.Quality					= 0;									// �摜�̕i�����x�����w��
		desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;		// �o�b�N�o�b�t�@�̎g�p���@���w��
		desc.BufferCount						= FrameCount;							// �o�b�N�o�b�t�@�̐����w��i�_�u���o�b�t�@�����O�Ȃ̂�2�j
		desc.OutputWindow						= m_hWnd;								// �o�͂���E�C���h�E�ւ̃n���h�����w��
		desc.Windowed							= TRUE;
		desc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �`�悪�I�������Ƀo�b�N�o�b�t�@�̓��e��j��
		desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;// ����I�v�V�������w��

		// �X���b�v�`�F�C���̐���
		IDXGISwapChain* pSwapChain = nullptr;
		hr = pFactory->CreateSwapChain(m_pQueue, &desc, &pSwapChain);
		if(FAILED(hr))
		{
			SafeRelease(pFactory);
			return false;
		}

		// IDXGISwapChain3���擾
		hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));	// �o�b�N�o�b�t�@�ԍ��̎擾���\�b�h���擾
		if (FAILED(hr))
		{
			SafeRelease(pFactory);
			SafeRelease(pSwapChain);
			return false;
		}

		// �o�b�N�o�b�t�@�ԍ����擾
		m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		// �s�v�ɂȂ����̂ŉ��
		SafeRelease(pFactory);
		SafeRelease(pSwapChain);
	}

	// �R�}���h�A���[�P�[�^�̐����i�R�}���h���X�g���g�p���郁���������蓖�Ă��邽�߂ɃR�}���h�A���[�P�[�^���쐬�j
	{
		// �_�u���o�b�t�@�����O���g�p���邽��2�t���[�����̃R�}���h�A���[�P�[�^�𐶐�
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

	// �R�}���h���X�g�̐���
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

	// �����_�[�^�[�Q�b�g�r���[�̐����i�`���j
	{
		// �f�B�X�N���v�^�q�[�v�̐ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors				= FrameCount;
		desc.Type						= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags						= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask					= 0;

		// �f�B�X�N���v�^�q�[�v�𐶐�
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

			// �����_�[�^�[�Q�b�g�r���[�̐���
			m_pDevice->CreateRenderTargetView(m_pColorBuffer[i], &viewDesc, handle);

			m_HandleRTV[i] = handle;
			handle.ptr += incrementSize;
		}
	}

	// �t�F���X
	{
		// �t�F���X�̃J�E���^�[�����Z�b�g
		for (auto i = 0; i < FrameCount; ++i)
		{
			m_FenceCounter[i] = 0;
		}

		// �t�F���X�̐���
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

	// �R�}���h���X�g�����
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
	// �V�[���Ǘ��N���X�����
	if (m_sceneManager != nullptr)
	{
		delete m_sceneManager;
	}

	// �E�B���h�E�̓o�^������
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd  = nullptr;
}

void App::StartApp(const TCHAR* appName)
{
	// �E�B���h�E����
	InitWindow(appName);

	// �V�[���Ǘ��N���X����
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
