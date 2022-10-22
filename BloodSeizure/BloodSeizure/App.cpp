#include "App.h"
#include <assert.h>
#include <string>

const auto ClassName = TEXT("BloodSeizure");


App::App(uint32_t width, uint32_t height)
    : m_hInst(nullptr)
    , m_hWnd(nullptr)
    , m_Width(width)
    , m_Height(height)
    , m_pDevice(nullptr)
    , m_pQueue(nullptr)
    , m_pSwapChain(nullptr)
    , m_pCmdList(nullptr)
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

	// ����I��.
	return true;
}

void App::TermApp()
{
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
            //Render();
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
        desc.BufferDesc.Width = m_Width;
        desc.BufferDesc.Height = m_Height;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = FrameCount;
        desc.OutputWindow = m_hWnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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

    // �����_�[�^�[�Q�b�g�r���[�̐���.
    {
        // �f�B�X�N���v�^�q�[�v�̐ݒ�.
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = FrameCount;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0;

        // �f�B�X�N���v�^�q�[�v�𐶐�.
        hr = m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_pHeapRTV.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        auto handle = m_pHeapRTV->GetCPUDescriptorHandleForHeapStart();
        auto incrementSize = m_pDevice
            ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (auto i = 0u; i < FrameCount; ++i)
        {
            hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(m_pColorBuffer[i].GetAddressOf()));
            if (FAILED(hr))
            {
                return false;
            }

            D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
            viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            viewDesc.Texture2D.MipSlice = 0;
            viewDesc.Texture2D.PlaneSlice = 0;

            // �����_�[�^�[�Q�b�g�r���[�̐���.
            m_pDevice->CreateRenderTargetView(m_pColorBuffer[i].Get(), &viewDesc, handle);

            m_HandleRTV[i] = handle;
            handle.ptr += incrementSize;
        }
    }

    // �t�F���X�̐���.
    {
        // �t�F���X�J�E���^�[�����Z�b�g.
        for (auto i = 0u; i < FrameCount; ++i)
        {
            m_FenceCounter[i] = 0;
        }

        // �t�F���X�̐���.
        hr = m_pDevice->CreateFence(
            m_FenceCounter[m_FrameIndex],
            D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(m_pFence.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        m_FenceCounter[m_FrameIndex]++;

        // �C�x���g�̐���.
        m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_FenceEvent == nullptr)
        {
            return false;
        }
    }

    // �R�}���h���X�g�����.
    m_pCmdList->Close();

    return true;
}

void App::TermD3D()
{
    // GPU�����̊�����ҋ@.
    WaitGpu();

    // �C�x���g�j��.
    if (m_FenceEvent != nullptr)
    {
        CloseHandle(m_FenceEvent);
        m_FenceEvent = nullptr;
    }

    // �t�F���X�j��.
    m_pFence.Reset();

    // �����_�[�^�[�Q�b�g�r���[�̔j��.
    m_pHeapRTV.Reset();
    for (auto i = 0u; i < FrameCount; ++i)
    {
        m_pColorBuffer[i].Reset();
    }

    // �R�}���h���X�g�̔j��.
    m_pCmdList.Reset();

    // �R�}���h�A���P�[�^�̔j��.
    for (auto i = 0u; i < FrameCount; ++i)
    {
        m_pCmdAllocator[i].Reset();
    }

    // �X���b�v�`�F�C���̔j��.
    m_pSwapChain.Reset();

    // �R�}���h�L���[�̔j��.
    m_pQueue.Reset();

    // �f�o�C�X�̔j��.
    m_pDevice.Reset();
}

void App::Render()
{
    // �R�}���h�̋L�^���J�n.
    m_pCmdAllocator[m_FrameIndex]->Reset();
    m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex].Get(), nullptr);

    // ���\�[�X�o���A�̐ݒ�.
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // ���\�[�X�o���A.
    m_pCmdList->ResourceBarrier(1, &barrier);

    // �����_�[�Q�b�g�̐ݒ�.
    m_pCmdList->OMSetRenderTargets(1, &m_HandleRTV[m_FrameIndex], FALSE, nullptr);

    // �N���A�J���[�̐ݒ�.
    float clearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

    // �����_�[�^�[�Q�b�g�r���[���N���A.
    m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex], clearColor, 0, nullptr);

    // �`�揈��.
    {
        // TODO : �|���S���`��p�̏�����ǉ�
        m_pCmdList->SetGraphicsRootSignature(m_pRootSignature.Get());
        m_pCmdList->SetDescriptorHeaps(1, m_pHeapCBV.GetAddressOf());
        m_pCmdList->SetGraphicsRootConstantBufferView(0, m_CBV[m_FrameIndex].Desc.BufferLocation);
        m_pCmdList->SetPipelineState(m_pPSO.Get());

        m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pCmdList->IASetVertexBuffers(0, 1, &m_VBV);
        m_pCmdList->RSSetViewports(1, &m_Viewport);
        m_pCmdList->RSSetScissorRects(1, &m_Scissor);

        m_pCmdList->DrawInstanced(3, 1, 0, 0);
    }

    // ���\�[�X�o���A�̐ݒ�.
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // ���\�[�X�o���A.
    m_pCmdList->ResourceBarrier(1, &barrier);

    // �R�}���h�̋L�^���I��.
    m_pCmdList->Close();

    // �R�}���h�����s.
    ID3D12CommandList* ppCmdLists[] = { m_pCmdList.Get() };
    m_pQueue->ExecuteCommandLists(1, ppCmdLists);

    // ��ʂɕ\��.
    Present(1);
}

void App::WaitGpu()
{
    assert(m_pQueue != nullptr);
    assert(m_pFence != nullptr);
    assert(m_FenceEvent != nullptr);

    // �V�O�i������.
    m_pQueue->Signal(m_pFence.Get(), m_FenceCounter[m_FrameIndex]);

    // �������ɃC�x���g��ݒ肷��..
    m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);

    // �ҋ@����.
    WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);

    // �J�E���^�[�𑝂₷.
    m_FenceCounter[m_FrameIndex]++;
}

void App::Present(uint32_t interval)
{
    // ��ʂɕ\��.
    m_pSwapChain->Present(interval, 0);

    // �V�O�i������.
    const auto currentValue = m_FenceCounter[m_FrameIndex];
    m_pQueue->Signal(m_pFence.Get(), currentValue);

    // �o�b�N�o�b�t�@�ԍ����X�V.
    m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
    if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex])
    {
        m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex], m_FenceEvent);
        WaitForSingleObjectEx(m_FenceEvent, INFINITE, FALSE);
    }

    // ���̃t���[���̃t�F���X�J�E���^�[�𑝂₷.
    m_FenceCounter[m_FrameIndex] = currentValue + 1;
}

bool App::OnInit()
{
    // ���_�o�b�t�@�̐���
    {
        // ���_�f�[�^
        Vertex vertices[] = {
            {DirectX::XMFLOAT3(-1.0f,-1.0f,0.0f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
            {DirectX::XMFLOAT3( 1.0f,-1.0f,0.0f),DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
            {DirectX::XMFLOAT3( 0.0f, 1.0f,0.0f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},
        };

        // �q�[�v�v���p�e�B
        D3D12_HEAP_PROPERTIES prop = {};
        prop.Type                   = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty        = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference   = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask       = 1;
        prop.VisibleNodeMask        = 1;

        // ���\�[�X�̐ݒ�
        D3D12_RESOURCE_DESC desc= {};
        desc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment          = 0;
        desc.Width              = sizeof(vertices);
        desc.Height             = 1;
        desc.DepthOrArraySize   = 1;
        desc.MipLevels          = 1;
        desc.Format             = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags              = D3D12_RESOURCE_FLAG_NONE;

        // ���\�[�X�̐���
        auto hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(m_pVB.GetAddressOf())
        );
        if (FAILED(hr))
        {
            return false;
        }

        // �}�b�s���O������
        void* ptr = nullptr;
        hr = m_pVB->Map(0, nullptr, &ptr);
        if (FAILED(hr))
        {
            return false;
        }

        // ���_�f�[�^���}�b�s���O��ɐݒ�
        memcpy(ptr, vertices, sizeof(vertices));

        // �}�b�s���O����
        m_pVB->Unmap(0, nullptr);

        // ���_�o�b�t�@�r���[�̐ݒ�
        m_VBV.BufferLocation = m_pVB->GetGPUVirtualAddress();
        m_VBV.StrideInBytes  = static_cast<UINT>(sizeof(vertices));
        m_VBV.StrideInBytes  = static_cast<UINT>(sizeof(Vertex));
    }

    // �萔�o�b�t�@�p�f�B�X�N���v�^�q�[�v�̐���
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1 * FrameCount;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask       = 0;

        auto hr = m_pDevice->CreateDescriptorHeap(
            &desc,
            IID_PPV_ARGS(m_pHeapCBV.GetAddressOf())
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �萔�o�b�t�@�̐���
    {
        // �q�[�v�v���p�e�B
        D3D12_HEAP_PROPERTIES prop  = {};
        prop.Type                   = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty        = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference   = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask       = 1;
        prop.VisibleNodeMask        = 1;

        // ���\�[�X�̐ݒ�
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension           = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment           = 0;
        desc.Width               = sizeof(Transform);
        desc.Height              = 1;
        desc.DepthOrArraySize    = 1;
        desc.MipLevels           = 1;
        desc.Format              = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count    = 1;
        desc.SampleDesc.Quality  = 0;
        desc.Layout              = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags               = D3D12_RESOURCE_FLAG_NONE;

        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (auto i = 0; i < FrameCount; ++i)
        {
            // ���\�[�X����
            auto hr = m_pDevice->CreateCommittedResource(
                &prop,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(m_pCB[i].GetAddressOf())
            );
            if (FAILED(hr))
            {
                return false;
            }

            auto address   = m_pCB[i]->GetGPUVirtualAddress();
            auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
            auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

            handleCPU.ptr += incrementSize * i;
            handleGPU.ptr += incrementSize * i;

            // �萔�o�b�t�@�r���[�̐ݒ�
            m_CBV[i].HandleCPU           = handleCPU;
            m_CBV[i].HandleGPU           = handleGPU;
            m_CBV[i].Desc.BufferLocation = address;
            m_CBV[i].Desc.SizeInBytes    = sizeof(Transform);

            // �萔�o�b�t�@�r���[�̐���
            m_pDevice->CreateConstantBufferView(&m_CBV[i].Desc, handleCPU);

            // �}�b�s���O
            hr = m_pCB[i]->Map(0, nullptr, reinterpret_cast<void**>(&m_CBV[i].pBuffer));
            if (FAILED(hr))
            {
                return false;
            }

            auto eyePos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
            auto targetPos = DirectX::XMVectorZero();
            auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            auto fovY = DirectX::XMConvertToRadians(37.5f);
            auto aspect = static_cast<float>(m_Width) / static_cast<float>(m_Height);

            // �ϊ��s��̐ݒ�
            m_CBV[i].pBuffer->World = DirectX::XMMatrixIdentity();
            m_CBV[i].pBuffer->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
            m_CBV[i].pBuffer->Proj = DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, 1.0f, 1000.0f);
        }
    }

    // ���[�g�V�O�j�`��
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag != D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag != D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag != D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        // ���[�g�p�����[�^�̐ݒ�
        D3D12_ROOT_PARAMETER param      = {};
        param.ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param.Descriptor.ShaderRegister = 0;
        param.Descriptor.RegisterSpace  = 0;
        param.ShaderVisibility          = D3D12_SHADER_VISIBILITY_VERTEX;

        // ���[�g�V�O�j�`��
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters             = 1;
        desc.NumStaticSamplers         = 0;
        desc.pParameters               = &param;
        desc.pStaticSamplers           = nullptr;
        desc.Flags                     = flag;

        ComPtr<ID3DBlob> pBlob;
        ComPtr<ID3DBlob> pErrorBlob;

        // �V���A���C�Y
        auto hr = D3D12SerializeRootSignature(
            &desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf()
        );
        if (FAILED(hr))
        {
            return false;
        }

        // ���[�g�V�O�j�`���𐶐�
        hr = m_pDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(m_pRootSignature.GetAddressOf())
        );
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �p�C�v���C���X�e�[�g�̐���
    {
        // ���̓��C�A�E�g�̐ݒ�
        D3D12_INPUT_ELEMENT_DESC elemnts[2];
        elemnts[0].SemanticName         = "POSITION";
        elemnts[0].SemanticIndex        = 0;
        elemnts[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        elemnts[0].InputSlot            = 0;
        elemnts[0].AlignedByteOffset    = D3D12_APPEND_ALIGNED_ELEMENT;
        elemnts[0].InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        elemnts[0].InstanceDataStepRate = 0;

        elemnts[1].SemanticName         = "COLOR";
        elemnts[1].SemanticIndex        = 0;
        elemnts[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
        elemnts[1].InputSlot            = 0;
        elemnts[1].AlignedByteOffset    = D3D12_APPEND_ALIGNED_ELEMENT;
        elemnts[1].InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        elemnts[1].InstanceDataStepRate = 0;

        // ���X�^���C�U�[�X�e�[�g�̐ݒ�
        D3D12_RASTERIZER_DESC descRS;
        descRS.FillMode              = D3D12_FILL_MODE_SOLID;
        descRS.CullMode              = D3D12_CULL_MODE_NONE;
        descRS.FrontCounterClockwise = FALSE;
        descRS.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
        descRS.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        descRS.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        descRS.DepthClipEnable       = FALSE;
        descRS.MultisampleEnable     = FALSE;
        descRS.AntialiasedLineEnable = FALSE;
        descRS.ForcedSampleCount     = 0;
        descRS.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
        D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
            FALSE,FALSE,
            D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL
        };

        // �u�����h�X�e�[�g�̐ݒ�
        D3D12_BLEND_DESC descBS;
        descBS.AlphaToCoverageEnable  = FALSE;
        descBS.IndependentBlendEnable = FALSE;
        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            descBS.RenderTarget[i] = descRTBS;
        }

        ComPtr<ID3DBlob> pVSBlob;
        ComPtr<ID3DBlob> pPSBlob;

        std::wstring filePath = L"../x64/Debug/SimpleVS.cso";

        // ���_�V�F�[�_�[�ǂݍ���
        auto hr = D3DReadFileToBlob(filePath.c_str(), pVSBlob.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        filePath = L"../x64/Debug/SimplePS.cso";

        // �s�N�Z���V�F�[�_�[�ǂݍ���
        hr = D3DReadFileToBlob(filePath.c_str(), pPSBlob.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        // �p�C�v���C���X�e�[�g�̐ݒ�
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout                     = { elemnts,_countof(elemnts) };
        desc.pRootSignature                  = m_pRootSignature.Get();
        desc.VS                              = { pVSBlob->GetBufferPointer(),pVSBlob->GetBufferSize() };
        desc.PS                              = { pPSBlob->GetBufferPointer(),pPSBlob->GetBufferSize() };
        desc.RasterizerState                 = descRS;
        desc.BlendState                      = descBS;
        desc.DepthStencilState.DepthEnable   = FALSE;
        desc.DepthStencilState.StencilEnable = FALSE;
        desc.SampleMask                      = UINT_MAX;
        desc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets                = 1;
        desc.RTVFormats[0]                   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.DSVFormat                       = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count                = 1;
        desc.SampleDesc.Quality              = 0;

        // �p�C�v���C���X�e�[�g�̐���
        hr = m_pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }
    }

    // �r���[�|�[�g�ƃV�U�[��`�̐ݒ�
    {
        m_Viewport.TopLeftX = 0;
        m_Viewport.TopLeftY = 0;
        m_Viewport.Width = static_cast<float>(m_Width);
        m_Viewport.Height = static_cast<float>(m_Height);
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;

        m_Scissor.left = 0;
        m_Scissor.right = m_Width;
        m_Scissor.top = 0;
        m_Scissor.bottom = m_Height;
    }

    return true;
}

void App::OnTerm()
{
    for (auto i = 0; i < FrameCount; ++i)
    {
        if (m_pCB[i].Get() != nullptr)
        {
            m_pCB[i]->Unmap(0, nullptr);
            memset(&m_CBV[i], 0, sizeof(m_CBV[i]));
        }
        m_pCB[i].Reset();
    }

    //m_pIB.Reset();
    m_pVB.Reset();
    m_pPSO.Reset();
}

LRESULT App::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:
    { PostQuitMessage(0); }
    break;

    default:
    { /* DO_NOTHING */ }
    break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
