#pragma once
#include "App.h"

class Player : public App
{
public:
    Player(uint32_t width, uint32_t height);
    virtual ~Player();

private:

    ComPtr<ID3D12PipelineState> m_pPipelineState;
    ComPtr<ID3D12RootSignature> m_pRootSignature;

    bool OnInit() override;

    void OnTerm() override;

    void OnRender() override;
};