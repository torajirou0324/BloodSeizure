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
	char c;
	std::cin >> c;
	if (c == 'a')
	{
		return TAG_SCENE::RESULT;
	}
	if (c == 'd')
	{
		return TAG_SCENE::END;
	}
	return TAG_SCENE::NONE;
}

void PlayScene::Draw()
{
	std::cout << "PLAYƒV[ƒ“" << std::endl;
}
