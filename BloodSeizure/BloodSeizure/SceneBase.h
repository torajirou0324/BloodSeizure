#pragma once

// シーン名のタグ
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

// シーンクラスの基底クラス
class SceneBase
{
public:
	SceneBase(){}
	virtual ~SceneBase(){}

	virtual TAG_SCENE Update() = 0;
	virtual void Draw() = 0;
};