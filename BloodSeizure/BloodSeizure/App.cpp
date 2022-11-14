#include "App.h"
#include <assert.h>
#include <string>

const auto ClassName = TEXT("BloodSeizure");

App::App(uint32_t width, uint32_t height)
    : m_hInst(nullptr)
    , m_hWnd(nullptr)
    , m_Width(width)
    , m_Height(height)
    , m_FrameIndex(0)
{
}

App::~App()
{
}

void App::Run()
{
    if (InitApp())
    {
        MainLoop();
    }

    TermApp();
}

bool App::InitApp()
{
	// �E�B���h�E�̏�����.
	if (!InitWnd())
	{
		return false;
	}

	// Direct3D 12�̏�����.
	if (!InitD3D())
	{
		return false;
	}

    // �A�v���P�[�V����������
    if (!OnInit())
    {
        return false;
    }

	// ����I��.
	return true;
}

void App::TermApp()
{
    // �A�v���P�[�V�����I������
    OnTerm();

	// Direct3D 12�̏I������.
	TermD3D();

	// �E�B���h�E�̏I������.
	TermWnd();
}

bool App::InitWnd()
{
    // �C���X�^���X�n���h�����擾.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    {
        return false;
    }

    // �E�B���h�E�̐ݒ�.
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = ClassName;
    wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

    // �E�B���h�E�̓o�^.
    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    // �C���X�^���X�n���h���ݒ�.
    m_hInst = hInst;

    // �E�B���h�E�̃T�C�Y��ݒ�.
    RECT rc = {};
    rc.right = static_cast<LONG>(m_Width);
    rc.bottom = static_cast<LONG>(m_Height);

    // �E�B���h�E�T�C�Y�𒲐�.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // �E�B���h�E�𐶐�.
    m_hWnd = CreateWindowEx(
        0,
        ClassName,
        TEXT("Sample"),
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (m_hWnd == nullptr)
    {
        return false;
    }

    // �E�B���h�E��\��.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // �E�B���h�E���X�V.
    UpdateWindow(m_hWnd);

    // �E�B���h�E�Ƀt�H�[�J�X��ݒ�.
    SetFocus(m_hWnd);

    // ����I��.
    return true;
}

void App::TermWnd()
{
    // �E�B���h�E�̓o�^������.
    if (m_hInst != nullptr)
    {
        UnregisterClass(ClassName, m_hInst);
    }

    m_hInst = nullptr;
    m_hWnd = nullptr;
}

void App::MainLoop()
{
    MSG msg = {};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            OnRender();
        }
    }
}

bool App::InitD3D()
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debug;
        auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));

        // �f�o�b�O���C���[��L����.
        if (SUCCEEDED(hr))
        {
            debug->EnableDebugLayer();
        }
    }
#endif

    // �f�o�C�X�̐���.
    auto hr = D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(m_pDevice.GetAddressOf()));
    if (FAILED(hr))
    {
        return false;
    }

    // �R�}���h�L���[�̐���.
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pQueue.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �X���b�v�`�F�C���̐���.
    {
        // DXGI�t�@�N�g���[�̐���.
        ComPtr<IDXGIFactory4> pFactory = nullptr;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(pFactory.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        // �X���b�v�`�F�C���̐ݒ�.
        DXGI_SWAP_CHAIN_DESC desc = {};
        desc.BufferDesc.Width                   = m_Width;
        desc.BufferDesc.Height                  = m_Height;
        desc.BufferDesc.RefreshRate.Numerator   = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count                   = 1;
        desc.SampleDesc.Quality                 = 0;
        desc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount                        = FrameCount;
        desc.OutputWindow                       = m_hWnd;
        desc.Windowed                           = TRUE;
        desc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // �X���b�v�`�F�C���̐���.
        ComPtr<IDXGISwapChain> pSwapChain;
        hr = pFactory->CreateSwapChain(m_pQueue.Get(), &desc, pSwapChain.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        // IDXGISwapChain3 ���擾.
        hr = pSwapChain.As(&m_pSwapChain);
        if (FAILED(hr))
        {
            return false;
        }

        // �o�b�N�o�b�t�@�ԍ����擾.
        m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

        // �s�v�ɂȂ����̂ŉ��.
        pFactory.Reset();
        pSwapChain.Reset();
    }

    // �f�B�X�N���v�^�v�[���̐���
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NodeMask = 1;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (!DescriptorPool::Create(m_pDevice.Get(), &desc, &m_pPool[POOL_TYPE_RES]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        desc.NumDescriptors = 256;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (!DescriptorPool::Create(m_pDevice.Get(), &desc, &m_pPool[POOL_TYPE_SMP]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (!DescriptorPool::Create(m_pDevice.Get(), &desc, &m_pPool[POOL_TYPE_RTV]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if(!DescriptorPool::Create(m_pDevice.Get(),&desc,&m_pPool[POOL_TYPE_DSV]))
        {
            return false;
        }
    }

    // �R�}���h���X�g�̐���
    {
        if (!m_CommandList.Init(
            m_pDevice.Get(),
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            FrameCount
        ))
        {
            return false;
        }
    }

    // �����_�[�^�[�Q�b�g�r���[�̐���.
    {
        for (auto i = 0u; i < FrameCount; ++i)
        {
            if (!m_ColorTarget[i].InitFromBackBuffer(
                m_pDevice.Get(),
                m_pPool[POOL_TYPE_RTV],
                i,
                m_pSwapChain.Get()
            ))
            {
                return false;
            }
        }
    }

    // �[�x�X�e���V���o�b�t�@�̐���
    {
        if (!m_DepthTarget.Init(
            m_pDevice.Get(),
            m_pPool[POOL_TYPE_DSV],
            m_Width,
            m_Height,
            DXGI_FORMAT_D32_FLOAT
        ))
        {
            return false;
        }
    }

    // �t�F���X�̐���.
    if (!m_Fence.Init(m_pDevice.Get()))
    {
        return false;
    }

    // �r���[�|�[�g�̐ݒ�
    {
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;
        m_Viewport.Width = float(m_Width);
        m_Viewport.Height = float(m_Height);
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;
    }

    // �V�U�[��`�̐ݒ�
    {
        m_Scissor.left = 0;
        m_Scissor.right = m_Width;
        m_Scissor.top = 0;
        m_Scissor.bottom = m_Height;
    }

    // ����I��
    return true;
}

void App::TermD3D()
{
    // GPU�����̊�����ҋ@.
    m_Fence.Sync(m_pQueue.Get());

    // �t�F���X�j��
    m_Fence.Term();

    // �����_�[�^�[�Q�b�g�r���[�̔j��.
    for (auto i = 0u; i < FrameCount; ++i)
    {
        m_ColorTarget[i].Term();
    }

    // �[�x�X�e���V���r���[�̔j��
    m_DepthTarget.Term();

    // �R�}���h���X�g�̔j��.
    m_CommandList.Term();

    for (auto i = 0; i < POOL_COUNT; ++i)
    {
        if (m_pPool[i] != nullptr)
        {
            m_pPool[i]->Release();
            m_pPool[i] = nullptr;
        }
    }

    // �X���b�v�`�F�C���̔j��.
    m_pSwapChain.Reset();

    // �R�}���h�L���[�̔j��.
    m_pQueue.Reset();

    // �f�o�C�X�̔j��.
    m_pDevice.Reset();
}

void App::Present(uint32_t interval)
{
    // ��ʂɕ\��.
    m_pSwapChain->Present(interval, 0);

    // �����҂�
    m_Fence.Wait(m_pQueue.Get(), INFINITE);

    // �t���[���ԍ����X�V
    m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
}