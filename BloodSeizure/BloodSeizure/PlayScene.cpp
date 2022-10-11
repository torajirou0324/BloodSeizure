#include "PlayScene.h"
#include <iostream>

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

TAG_SCENE PlayScene::Update()
{
	return TAG_SCENE::NONE;
}

void PlayScene::Draw()
{
	std::cout << "PLAYƒV[ƒ“" << std::endl;
}
