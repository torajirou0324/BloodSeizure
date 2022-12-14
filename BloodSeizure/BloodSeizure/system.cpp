#include "system.h"

HWND		g_hWnd = NULL;							// ウィンドウのハンドル
HINSTANCE	g_hInst = NULL;							// インスタンスハンドル
const TCHAR* CLASS_NAME = TEXT("BloodSeizure");     // クラスの名前

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
// ウィンドウの初期化
/////////////////////////////////////////////////
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// インスタンスハンドルを取得
	auto hInst = GetModuleHandle(nullptr);

	// ウィンドウクラスのパラメータを設定(構造体の変数の初期化)
	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);					// 構造体のサイズ
	wc.style			= CS_CLASSDC;							// ウィンドウスタイル
	wc.lpfnWndProc		= MsgProc;								// メッセージプロシージャ
	wc.cbClsExtra		= 0;									// 
	wc.cbWndExtra		= 0;									// 
	wc.hInstance		= hInst;								// このクラスのためのウィンドウプロシージャがあるインスタンスハンドル
	wc.hIcon			= LoadIcon(hInst, IDI_APPLICATION);		// アイコンのハンドル
	wc.hCursor			= LoadCursor(hInst, IDC_ARROW);			// マウスカーソルのハンドル。NULLなのでデフォルト
	wc.hbrBackground	= GetSysColorBrush(COLOR_BACKGROUND);	// ウィンドウの背景色。NULLなのでデフォルト
	wc.lpszMenuName		= NULL;									// メニュー名。
	wc.lpszClassName	= CLASS_NAME;								// ウィンドウクラスに付ける名前
	wc.hIconSm			= LoadIcon(hInst, IDI_APPLICATION);		// 
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wc);

	// インスタンスハンドル設定
	g_hInst = hInst;

	// ウィンドウのサイズを設定
	RECT rc = {};
	rc.right = static_cast<LONG>(FRAME_BUFFER_W);
	rc.bottom = static_cast<LONG>(FRAME_BUFFER_H);

	// ウィンドウサイズを調整
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウクラスの作成
	g_hWnd = CreateWindow(
		CLASS_NAME,				// 使用するウィンドウクラスの名前
		CLASS_NAME,				// ウィンドウの名前。ウィンドウクラスの名前と別名でもよい
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル。
		0,						// ウィンドウの初期X座標
		0,						// ウィンドウの初期Y座標
		FRAME_BUFFER_W,			// ウィンドウの幅
		FRAME_BUFFER_H,			// ウィンドウの高さ
		NULL,					// 親ウィンドウ
		NULL,					// メニュー
		g_hInst,				// アプリケーションのインスタンス
		NULL					// 
	);

	ShowWindow(g_hWnd, nCmdShow);
}

void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウの初期化
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	// エンジンの初期化
	g_graphicsEngine = new GraphicsEngine;
	g_graphicsEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
}

void TermGame()
{
	if (g_graphicsEngine != nullptr)
	{
		delete g_graphicsEngine;
	}

	// ウィンドウの登録解除
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
		// ウィンドウからのメッセージを受け取る
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			// ウィンドウメッセージがからになった
			break;
		}
	}
	return msg.message != WM_QUIT;
}
