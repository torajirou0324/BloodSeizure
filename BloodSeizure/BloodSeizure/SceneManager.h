#pragma once
#include "SceneBase.h"

// シーンクラスを管理するクラス
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void GameLoop();

	void SetNowScene(const TAG_SCENE& tag);

private:
	void ClearScene();
	SceneBase* m_nowScene;
};
