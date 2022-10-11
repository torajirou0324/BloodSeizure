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
	return TAG_SCENE::NONE;
}

void ResultScene::Draw()
{
	std::cout << "RESULTƒV[ƒ“" << std::endl;
}
