#include "ResultScene.h"
#include <iostream>

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

TAG_SCENE ResultScene::Update()
{
	char c;
	std::cin >> c;
	if (c == 'a')
	{
		return TAG_SCENE::TITLE;
	}
	if (c == 'd')
	{
		return TAG_SCENE::END;
	}
	return TAG_SCENE::NONE;
}

void ResultScene::Draw()
{
	std::cout << "RESULTƒV[ƒ“" << std::endl;
}
