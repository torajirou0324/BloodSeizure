#pragma once
#include "GraphicsEngine.h"

// ゲームの初期化
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
// ウィンドウメッセージをディスパッチ。falseが帰ってきたらゲーム終了
bool DispatchWindowMessage();

const UINT FRAME_BUFFER_W = 1920;   // 画面の横幅
const UINT FRAME_BUFFER_H = 1080;   // 画面の縦幅