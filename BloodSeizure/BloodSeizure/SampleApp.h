#pragma once
#include "App.h"
#include "ConstantBuffer.h"
#include "Material.h"

class SampleApp : public App
{
public:
    SampleApp(uint32_t width, uint32_t height);
    virtual ~SampleApp();

private:
    std::vector<Mesh*>              m_pMesh;        // メッシュ
    std::vector<ConstantBuffer*>    m_Transform;   // 変換行列
    ConstantBuffer*                 m_pLight;       // ライト
    Material                        m_Material;     // マテリアル
    ComPtr<ID3D12PipelineState>     m_pPSO;         // パイプラインステート
    ComPtr<ID3D12RootSignature>     m_pRootSig;     // ルートシグネチャ
    float                           m_RotateAngle;  // 回転角度

    // 初期化処理
    bool OnInit() override;

    // 解放処理
    void OnTerm() override;

    // 描画処理
    void OnRender() override;
};