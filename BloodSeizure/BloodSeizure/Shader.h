#pragma once

class Shader
{
public:
    void LoadPS(const char* filePath, const char* entryFuncName);

    void LoadVS(const char* filePath, const char* entryFuncName);
};