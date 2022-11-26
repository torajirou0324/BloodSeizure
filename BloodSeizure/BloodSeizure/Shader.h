#pragma once

class Shader
{
public:
    /// <summary>
    /// ピクセルシェーダをロード
    /// </summary>
    /// <param name="filePath">ファイルパス。</param>
    /// <param name="entryFuncName">エントリーポイントの関数名。</param>
    void LoadPS(const char* filePath, const char* entryFuncName);

    /// <summary>
    /// 頂点シェーダをロード
    /// </summary>
    /// <param name="filePath">ファイルパス。</param>
    /// <param name="entryFuncName">エントリーポイントの関数名。</param>
    void LoadVS(const char* filePath, const char* entryFuncName);

    /// <summary>
    /// コンパイル済み頂点シェーダーのデータを取得
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledVSBlob() const
    {
        return m_vsBlob;
    }

    /// <summary>
    /// コンパイル済みのピクセルシェーダーのデータを取得
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledPSBlob() const
    {
        return m_psBlob;
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
    ID3DBlob* m_vsBlob = nullptr;   // コンパイル済みの頂点シェーダーのデータ
    ID3DBlob* m_psBlob = nullptr;   // コンパイル済みのピクセルシェーダーのデータ
    bool m_isInited = false;        // 初期化済みかどうか
};