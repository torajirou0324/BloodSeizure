#pragma once
#include "GraphicsEngine.h"
#include "Shader.h"

class Player
{
public:
    Player();
    virtual ~Player();

private:
    Shader m_Shader;
};