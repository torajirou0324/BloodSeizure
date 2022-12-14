#include "system.h"

HWND		g_hWnd = NULL;							// �E�B���h�E�̃n���h��
HINSTANCE	g_hInst = NULL;							// �C���X�^���X�n���h��
const TCHAR* CLASS_NAME = TEXT("BloodSeizure");     // �N���X�̖��O

/////////////////////////////////////////////////
// ���b�Z�[�W�v���V�[�W���[
// hWnd�����b�Z�[�W�𑗂��Ă����E�B���h�E�n���h��
// msg�����b�Z�[�W�̎��
/////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // �����Ă������b�Z�[�W�ŏ����𕪊򂳂���
	switch (msg)
	{
	case WM_DESTROY:
		// �G���W���̔j��
		if (g_graphicsEngine != nullptr)
		{
			TermGame();
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////
// �E�B���h�E�̏�����
/////////////////////////////////////////////////
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �C���X�^���X�n���h�����擾
	auto hInst = GetModuleHandle(nullptr);

	// �E�B���h�E�N���X�̃p�����[�^��ݒ�(�\���̂̕ϐ��̏�����)
	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);					// �\���̂̃T�C�Y
	wc.style			= CS_CLASSDC;							// �E�B���h�E�X�^�C��
	wc.lpfnWndProc		= MsgProc;								// ���b�Z�[�W�v���V�[�W��
	wc.cbClsExtra		= 0;									// 
	wc.cbWndExtra		= 0;									// 
	wc.hInstance		= hInst;								// ���̃N���X�̂��߂̃E�B���h�E�v���V�[�W��������C���X�^���X�n���h��
	wc.hIcon			= LoadIcon(hInst, IDI_APPLICATION);		// �A�C�R���̃n���h��
	wc.hCursor			= LoadCursor(hInst, IDC_ARROW);			// �}�E�X�J�[�\���̃n���h���BNULL�Ȃ̂Ńf�t�H���g
	wc.hbrBackground	= GetSysColorBrush(COLOR_BACKGROUND);	// �E�B���h�E�̔w�i�F�BNULL�Ȃ̂Ńf�t�H���g
	wc.lpszMenuName		= NULL;									// ���j���[���B
	wc.lpszClassName	= CLASS_NAME;								// �E�B���h�E�N���X�ɕt���閼�O
	wc.hIconSm			= LoadIcon(hInst, IDI_APPLICATION);		// 
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �C���X�^���X�n���h���ݒ�
	g_hInst = hInst;

	// �E�B���h�E�̃T�C�Y��ݒ�
	RECT rc = {};
	rc.right = static_cast<LONG>(FRAME_BUFFER_W);
	rc.bottom = static_cast<LONG>(FRAME_BUFFER_H);

	// �E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// �E�B���h�E�N���X�̍쐬
	g_hWnd = CreateWindow(
		CLASS_NAME,				// �g�p����E�B���h�E�N���X�̖��O
		CLASS_NAME,				// �E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C���B
		0,						// �E�B���h�E�̏���X���W
		0,						// �E�B���h�E�̏���Y���W
		FRAME_BUFFER_W,			// �E�B���h�E�̕�
		FRAME_BUFFER_H,			// �E�B���h�E�̍���
		NULL,					// �e�E�B���h�E
		NULL,					// ���j���[
		g_hInst,				// �A�v���P�[�V�����̃C���X�^���X
		NULL					// 
	);

	ShowWindow(g_hWnd, nCmdShow);
}

void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E�̏�����
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	// �G���W���̏�����
	g_graphicsEngine = new GraphicsEngine;
	g_graphicsEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
}

void TermGame()
{
	if (g_graphicsEngine != nullptr)
	{
		delete g_graphicsEngine;
	}

	// �E�B���h�E�̓o�^����
	if (g_hInst != nullptr)
	{
		UnregisterClass(CLASS_NAME, g_hInst);
	}

	g_hInst = nullptr;
	g_hWnd = nullptr;
}

bool DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		// �E�B���h�E����̃��b�Z�[�W���󂯎��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			// �E�B���h�E���b�Z�[�W������ɂȂ���
			break;
		}
	}
	return msg.message != WM_QUIT;
}
