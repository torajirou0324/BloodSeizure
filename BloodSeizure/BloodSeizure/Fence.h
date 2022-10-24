#pragma once

#include <d3d12.h>
#include "ComPtr.h"

class Fence
{
public:
    Fence();
    ~Fence();

    // ����������
    bool Init(ID3D12Device* pDevice);

    // �������
    void Term();

    // �V�O�i����ԂɂȂ�܂Ŏw�肳�ꂽ���ԑҋ@
    void Wait(ID3D12CommandQueue* pQueue, UINT timeout);

    // �V�O�i����ԂɂȂ�܂ł����Ƒҋ@
    void Sync(ID3D12CommandQueue* pQueue);

private:
    ComPtr<ID3D12Fence> m_pFence;   // �t�F���X
    HANDLE              m_Event;    // �C�x���g
    UINT                m_Counter;  // ���݂̃J�E���^�[

    Fence(const Fence&) = delete;
    void operator = (const Fence&) = delete;
};