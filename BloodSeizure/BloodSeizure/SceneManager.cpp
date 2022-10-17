#include "SceneManager.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

SceneManager::SceneManager()
	: m_nowScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	ClearScene();
}

void SceneManager::GameLoop()
{
	while (1)
	{
		// 現在のシーン描画処理
		m_nowScene->Draw();
		// 現在のシーン更新処理
		auto tag = m_nowScene->Update();

		if (tag == TAG_SCENE::NONE)
		{
			return;		// TAGがNONEなのでシーンに変更はないのでループを続ける。
		}

		// 現在のシーンを解放する
		ClearScene();
		if (tag == TAG_SCENE::END)
		{
			break;		// アプリケーション終了のタグが返ってたのでループを抜ける。
		}
		//	次のシーンを生成する。
		SetNowScene(tag);
	}
}

void SceneManager::SetNowScene(const TAG_SCENE& tag)
{
	switch (tag)
	{
	case TAG_SCENE::TITLE:
		m_nowScene = new TitleScene;
		break;
	case TAG_SCENE::PLAY:
		m_nowScene = new PlayScene;
		break;
	case TAG_SCENE::RESULT:
		m_nowScene = new ResultScene;
	default:
		break;
	}
}

void SceneManager::ClearScene()
{
	// 中身がnullptrじゃないとき
	if (m_nowScene != nullptr)
	{
		delete m_nowScene;	// 解放する
		m_nowScene = nullptr;
	}
}
