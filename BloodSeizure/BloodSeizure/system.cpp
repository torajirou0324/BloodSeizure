#include "system.h"

HWND g_hWnd = NULL;     // ウィンドウのハンドル

/////////////////////////////////////////////////
// メッセージプロシージャー
// hWndがメッセージを送ってきたウィンドウハンドル
// msgがメッセージの種類
/////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // 送られてきたメッセージで処理を分岐させる
	switch (msg)
	{
	case WM_DESTROY:
		// エンジンの破棄
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
/// ウィンドウの初期化
/// </summary>
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	// ウィンドウクラスのパラメータを設定(構造体の変数の初期化)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ
		CS_CLASSDC,				// ウィンドウスタイル
		MsgProc,				// メッセージプロシージャ
		0,						// 
		0,						// 
		GetModuleHandle(NULL),	// このクラスのためのウィンドウプロシージャがあるインスタンスハンドル
		NULL,					// アイコンのハンドル
		NULL,					// マウスカーソルのハンドル。NULLなのでデフォルト
		NULL,					// ウィンドウの背景色。NULLなのでデフォルト
		NULL,					// メニュー名。
		appName,				// ウィンドウクラスに付ける名前
		NULL					// 
	};
	// ウィンドウクラスの登録
	RegisterClassEx(&wc);

	// ウィンドウクラスの作成
	g_hWnd = CreateWindow(
		appName,				// 使用するウィンドウクラスの名前
		appName,				// ウィンドウの名前。ウィンドウクラスの名前と別名でもよい
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル。
		0,						// ウィンドウの初期X座標
		0,						// ウィンドウの初期Y座標
		FRAME_BUFFER_W,			// ウィンドウの幅
		FRAME_BUFFER_H,			// ウィンドウの高さ
		NULL,					// 親ウィンドウ
		NULL,					// メニュー
		hInstance,				// アプリケーションのインスタンス
		NULL					// 
	);

	ShowWindow(g_hWnd, nCmdShow);
}

/// <summary>
/// ゲームの初期化
/// </summary>
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	// ウィンドウの初期化
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	// エンジンの初期化
	g_graphicsEngine = new GraphicsEngine;
	g_graphicsEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
}

/// <summary>
/// ウィンドウメッセージをディスパッチ。
/// </summary>
/// <returns>falseが返ってきたら、ゲーム終了。</returns>
bool DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		// ウィンドウからのメッセージを受け取る
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			// ウィンドウメッセージがからになった
		}
	}
	return msg.message != WM_QUIT;
}
