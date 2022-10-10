#pragma once
#include <Windows.h>
class SceneManager;

class App
{
public:
	App() = delete;
	App(const TCHAR* appName);
	~App();

	void GameLoop();
private:
	void StartApp(const TCHAR* appName);
	void InitWindow(const TCHAR* appName);

	SceneManager* m_sceneManager;

	const UINT WINDOW_WIDTH = 1280;
	const UINT WINDOW_HEIGHT = 720;
};
