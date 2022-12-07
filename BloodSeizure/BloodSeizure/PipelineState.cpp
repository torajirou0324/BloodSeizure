#include "PipelineState.h"
#include "CommonStates.h"

void PipelineState::Init(D3D12_INPUT_ELEMENT_DESC* inputElements, UINT elementSize, RootSignature& rs, Shader& vs, Shader& ps)
{
    D3D12_INPUT_LAYOUT_DESC InputLayout = { inputElements,elementSize };
    // グラフィックスパイプラインステートを設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.InputLayout            = InputLayout;
    desc.pRootSignature         = rs.Get();
    desc.VS                     = { vs.GetCompiledBlob()->GetBufferPointer(),vs.GetCompiledBlob()->GetBufferSize() };
    desc.PS                     = { ps.GetCompiledBlob()->GetBufferPointer(),ps.GetCompiledBlob()->GetBufferSize() };
    desc.RasterizerState        = DirectX::CommonStates::CullNone;
    desc.BlendState             = DirectX::CommonStates::Opaque;
    desc.DepthStencilState      = DirectX::CommonStates::DepthDefault;
    desc.SampleMask             = UINT_MAX;
    desc.PrimitiveTopologyType  = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets       = 1;
    desc.RTVFormats[0]          = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.DSVFormat              = DXGI_FORMAT_D32_FLOAT;
    desc.SampleDesc.Count       = 1;
    desc.SampleDesc.Quality     = 0;

    // パイプラインステートを生成
    auto device = g_graphicsEngine->GetD3DDevice();
    device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pPipelineState));
}
