#pragma once
#include "GraphicsEngine.h"

class RootSignature
{
public:
    RootSignature(){}
    ~RootSignature(){}

    void Init(D3D12_ROOT_PARAMETER* param, UINT paramNum);

    ID3D12RootSignature* Get()
    {
        return m_pRootSignature.Get();
    }

private:
    ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
};