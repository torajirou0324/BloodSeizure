#pragma once
#include "SceneBase.h"

// �V�[���N���X���Ǘ�����N���X
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
