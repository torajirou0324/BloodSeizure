#pragma once
#include "SceneBase.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene()override;

	TAG_SCENE Update() override;
	void      Draw()   override;
};