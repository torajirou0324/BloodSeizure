#include <iostream>
#include "App.h"
#include "SceneManager.h"

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
	HINSTANCE g_hInst = GetModuleHandle(nullptr);
	if (g_hInst == nullptr)
	{
		return;
	}

	// �E�B���h�E�̐ݒ�
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(g_hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = appName;
	wc.hIconSm = LoadIcon(g_hInst, IDI_APPLICATION);

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �E�B���h�E�T�C�Y�̐ݒ�
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);

	// �E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	// �E�B���h�E�̐���
	HWND g_hWnd = CreateWindowEx(
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
		g_hInst,
		nullptr
	);

	// �E�B���h�E��\��
	ShowWindow(g_hWnd, SW_SHOWNORMAL);

	// �E�B���h�E�Ƀt�H�[�J�X����
	SetFocus(g_hWnd);
}


App::App(const TCHAR* appName)
	: m_sceneManager(nullptr)
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
