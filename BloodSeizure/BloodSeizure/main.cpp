#include <stdio.h>
#include <stdlib.h>
#include "system.h"

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("BloodSeizure"));

	// ゲームループ
	while (DispatchWindowMessage())
	{
		// 1フレームの開始（シーンのクリア）
		g_graphicsEngine->BeginRender();



		// 1フレームの終了（表画面と裏画面を入れ替える）
		g_graphicsEngine->EndRender();
	}

	return 0;
}