#include <stdio.h>
#include <stdlib.h>
#include "system.h"

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏�����
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("BloodSeizure"));

	// �Q�[�����[�v
	while (DispatchWindowMessage())
	{
		// 1�t���[���̊J�n�i�V�[���̃N���A�j
		g_graphicsEngine->BeginRender();



		// 1�t���[���̏I���i�\��ʂƗ���ʂ����ւ���j
		g_graphicsEngine->EndRender();
	}

	return 0;
}