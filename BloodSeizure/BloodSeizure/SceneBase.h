#pragma once

// �V�[�����̃^�O
enum class TAG_SCENE
{
	TITLE,
	PLAY,
	RESULT,
	END,
	NONE,
};

enum class MODE
{

};

// �V�[���N���X�̊��N���X
class SceneBase
{
public:
	SceneBase(){}
	virtual ~SceneBase(){}

	virtual TAG_SCENE Update() = 0;
	virtual void Draw() = 0;
};