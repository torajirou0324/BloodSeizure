#pragma once
#include "GraphicsEngine.h"

class ConstantBuffer
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    ConstantBuffer();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ConstantBuffer();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="size">定数バッファのサイズ</param>
    /// <param name="srcData">ソースデータ。nullを指定することも可</param>
    void Init(int size, void* srcData = nullptr);

    /// <summary>
    /// 解放処理
    /// </summary>
    void Term();

    /// <summary>
    /// ディスクリプタヒープにConstantBufferViewを登録。
    /// </summary>
    /// <param name="descriptorHandle"></param>
    void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
    void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

    /// <summary>
    /// GPU仮想アドレスを取得する
    /// </summary>
    /// <returns></returns>
    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();

    /// <summary>
    /// データをVRAMにコピーする
    /// </summary>
    /// <param name="data"></param>
    void CopyToVRAM(void* data);
    template<class T>
    void CopyToVRAM(T& data)
    {
        CopyToVRAM(&data)
    }

private:
    ComPtr<ID3D12Resource>          m_pConstantBuffer[2] = { nullptr };         // 定数バッファ
    void*                           m_pConstantBufferCPU[2] = { nullptr };      // マップ済みポインタ
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_Desc;                                     // 定数バッファビュー構造体
    int                             m_size = 0;                                 // 定数バッファサイズ
    int                             m_alocSize = 0;                             // 
};