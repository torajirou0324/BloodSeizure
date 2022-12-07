#pragma once
#include "GraphicsEngine.h"
#include "RootSignature.h"
#include "Shader.h"

class PipelineState
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PipelineState(){}
    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PipelineState(){}

    /// <summary>
    /// パイプラインステートの初期化
    /// </summary>
    /// <param name="pipelineState">パイプラインステート</param>
    /// <param name="rs">ルートシグネチャ</param>
    /// <param name="vs">バーテックスシェーダー</param>
    /// <param name="ps">ピクセルシェーダー</param>
    void Init(D3D12_INPUT_ELEMENT_DESC* inputElements, UINT elementSize, RootSignature& rs, Shader& vs, Shader& ps);

    /// <summary>
    /// パイプラインステートの取得
    /// </summary>
    /// <returns></returns>
    ID3D12PipelineState* Get()
    {
        return m_pPipelineState;
    }

private:
    ID3D12PipelineState* m_pPipelineState = nullptr;    // パイプラインステート
};