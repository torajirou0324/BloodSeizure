#include "system.h"

HWND g_hWnd = NULL;     // �E�B���h�E�̃n���h��

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
			delete g_graphicsEngine;
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// �E�B���h�E�̏�����
/// </summary>
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	// �E�B���h�E�N���X�̃p�����[�^��ݒ�(�\���̂̕ϐ��̏�����)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// �\���̂̃T�C�Y
		CS_CLASSDC,				// �E�B���h�E�X�^�C��
		MsgProc,				// ���b�Z�[�W�v���V�[�W��
		0,						// 
		0,						// 
		GetModuleHandle(NULL),	// ���̃N���X�̂��߂̃E�B���h�E�v���V�[�W��������C���X�^���X�n���h��
		NULL,					// �A�C�R���̃n���h��
		NULL,					// �}�E�X�J�[�\���̃n���h���BNULL�Ȃ̂Ńf�t�H���g
		NULL,					// �E�B���h�E�̔w�i�F�BNULL�Ȃ̂Ńf�t�H���g
		NULL,					// ���j���[���B
		appName,				// �E�B���h�E�N���X�ɕt���閼�O
		NULL					// 
	};
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �E�B���h�E�N���X�̍쐬
	g_hWnd = CreateWindow(
		appName,				// �g�p����E�B���h�E�N���X�̖��O
		appName,				// �E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢
		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C���B
		0,						// �E�B���h�E�̏���X���W
		0,						// �E�B���h�E�̏���Y���W
		FRAME_BUFFER_W,			// �E�B���h�E�̕�
		FRAME_BUFFER_H,			// �E�B���h�E�̍���
		NULL,					// �e�E�B���h�E
		NULL,					// ���j���[
		hInstance,				// �A�v���P�[�V�����̃C���X�^���X
		NULL					// 
	);

	ShowWindow(g_hWnd, nCmdShow);
}

/// <summary>
/// �Q�[���̏�����
/// </summary>
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	// �E�B���h�E�̏�����
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	// �G���W���̏�����
	g_graphicsEngine = new GraphicsEngine;
	g_graphicsEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
}

/// <summary>
/// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`�B
/// </summary>
/// <returns>false���Ԃ��Ă�����A�Q�[���I���B</returns>
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
		}
	}
	return msg.message != WM_QUIT;
}
