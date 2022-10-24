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

    // ����������
    bool Init(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type, uint32_t count);

    // �������
    void Term();

    // ���Z�b�g�����s�����R�}���h���X�g��Ԃ�
    ID3D12GraphicsCommandList* Reset();

private:
    ComPtr<ID3D12GraphicsCommandList>           m_pCmdList;     // �R�}���h���X�g
    std::vector<ComPtr<ID3D12CommandAllocator>> m_pAllocators;  // �R�}���h�A���[�P�[�^
    uint32_t                                    m_Index;        // �A���[�P�[�^�ԍ�

    CommandList(const CommandList&) = delete;
    void operator = (const CommandList&) = delete;
};