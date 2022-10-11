#include "TitleScene.h"
#include <iostream>

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

TAG_SCENE TitleScene::Update()
{
	return TAG_SCENE::NONE;
}

void TitleScene::Draw()
{
	std::cout << "TITLEƒV[ƒ“" << std::endl;
}
