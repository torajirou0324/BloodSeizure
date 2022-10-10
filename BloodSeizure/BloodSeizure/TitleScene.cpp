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
	char c;
	std::cin >> c;
	if (c == 'a')
	{
		return TAG_SCENE::PLAY;
	}
	if (c == 'd')
	{
		return TAG_SCENE::END;
	}
	return TAG_SCENE::NONE;
}

void TitleScene::Draw()
{
	std::cout << "TITLEƒV[ƒ“" << std::endl;
}
