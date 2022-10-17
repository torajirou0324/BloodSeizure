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
		// ���݂̃V�[���`�揈��
		m_nowScene->Draw();
		// ���݂̃V�[���X�V����
		auto tag = m_nowScene->Update();

		if (tag == TAG_SCENE::NONE)
		{
			return;		// TAG��NONE�Ȃ̂ŃV�[���ɕύX�͂Ȃ��̂Ń��[�v�𑱂���B
		}

		// ���݂̃V�[�����������
		ClearScene();
		if (tag == TAG_SCENE::END)
		{
			break;		// �A�v���P�[�V�����I���̃^�O���Ԃ��Ă��̂Ń��[�v�𔲂���B
		}
		//	���̃V�[���𐶐�����B
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
	// ���g��nullptr����Ȃ��Ƃ�
	if (m_nowScene != nullptr)
	{
		delete m_nowScene;	// �������
		m_nowScene = nullptr;
	}
}
