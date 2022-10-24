#pragma once

#include <d3d12.h>
#include "ComPtr.h"

class Fence
{
public:
    Fence();
    ~Fence();

    // 初期化処理
    bool Init(ID3D12Device* pDevice);

    // 解放処理
    void Term();

    // シグナル状態になるまで指定された時間待機
    void Wait(ID3D12CommandQueue* pQueue, UINT timeout);

    // シグナル状態になるまでずっと待機
    void Sync(ID3D12CommandQueue* pQueue);

private:
    ComPtr<ID3D12Fence> m_pFence;   // フェンス
    HANDLE              m_Event;    // イベント
    UINT                m_Counter;  // 現在のカウンター

    Fence(const Fence&) = delete;
    void operator = (const Fence&) = delete;
};