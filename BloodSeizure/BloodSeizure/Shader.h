#pragma once
#include "GraphicsEngine.h"

class Shader
{
public:
    /// <summary>
    /// ピクセルシェーダをロード
    /// </summary>
    /// <param name="filePath">ファイルパス。</param>
    /// <param name="entryFuncName">エントリーポイントの関数名。</param>
    void LoadPS(const wchar_t* filePath);

    /// <summary>
    /// 頂点シェーダをロード
    /// </summary>
    /// <param name="filePath">ファイルパス。</param>
    /// <param name="entryFuncName">エントリーポイントの関数名。</param>
    void LoadVS(const wchar_t* filePath);

    /// <summary>
    /// コンパイル済み頂点シェーダーのデータを取得
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledBlob() const
    {
        return m_blob;
    }

    /// <summary>
    /// 初期化済みかどうかのフラグを取得
    /// </summary>
    /// <returns></returns>
    bool IsInited() const
    {
        return m_isInited;
    }

private:
    /// <summary>
    /// シェーダーをロード。
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    /// <param name="entryFuncName">エントリーポイントの関数名。</param>
    /// <param name="shaderModel">シェーダーモデル</param>
    void Load(const char* filePath, const char* entryFuncName, const char* shaderModel);

    ID3DBlob* m_blob = nullptr;   // コンパイル済みの頂点シェーダーのデータ
    bool m_isInited = false;        // 初期化済みかどうか
};