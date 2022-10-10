#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene()override;

	TAG_SCENE Update() override;
	void      Draw()   override;
};