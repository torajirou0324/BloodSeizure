#pragma once

#include <d3d12.h>
#include <cstdint>
#include <vector>
#include "ComPtr.h"

class CommandList
{
public:
    CommandList();
    ~CommandList();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type, uint32_t count);

    // 解放処理
    void Term();

    // リセット処理行ったコマンドリストを返す
    ID3D12GraphicsCommandList* Reset();

private:
    ComPtr<ID3D12GraphicsCommandList>           m_pCmdList;     // コマンドリスト
    std::vector<ComPtr<ID3D12CommandAllocator>> m_pAllocators;  // コマンドアローケータ
    uint32_t                                    m_Index;        // アローケータ番号

    CommandList(const CommandList&) = delete;
    void operator = (const CommandList&) = delete;
};