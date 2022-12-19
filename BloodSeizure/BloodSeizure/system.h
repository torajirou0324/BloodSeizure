#pragma once
#include <SimpleMath.h>
#include "GraphicsEngine.h"

// 頂点構造体
struct Vertex
{
    DirectX::XMFLOAT3 position;     // 座標
    DirectX::XMFLOAT4 color;        // カラー
};

/// <summary>
/// エンジンの初期化
/// </summary>
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

/// <summary>
/// エンジンの終了処理
/// </summary>
void TermGame();

/// <summary>
/// ウィンドウメッセージをディスパッチ。
/// </summary>
/// <returns>falseが返ってきたら、ゲーム終了。</returns>
bool DispatchWindowMessage();

const UINT FRAME_BUFFER_W = 1920;                   // 画面の横幅
const UINT FRAME_BUFFER_H = 1080;                   // 画面の縦幅

// GPUで使用する行列をまとめた構造体
struct Transform
{
    DirectX::SimpleMath::Matrix World;          // ワールド行列
    DirectX::SimpleMath::Matrix View;           // ビュー行列
    DirectX::SimpleMath::Matrix Proj;           // プロジェクション行列
};

struct LightBuffer
{
    DirectX::SimpleMath::Vector4 LightPosition; // ライト位置
    DirectX::SimpleMath::Color LightColor;      // ライトカラー
};

struct MaterialBuffer
{
    DirectX::SimpleMath::Vector3 Diffuse;       // 拡散反射率
    float Alpha;                                // 透過度
};