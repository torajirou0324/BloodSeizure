#pragma once

#include <d3d12.h>
#include <cstdint>
#include "ComPtr.h"

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    // ����������
    bool Init(ID3D12Device* pDevice, size_t size, const uint32_t* pInitData = nullptr);

    // �������
    void Term();

    // �������}�b�s���O
    uint32_t* Map();

    // �������}�b�s���O����
    void Unmap();

    // �C���f�b�N�X�o�b�t�@�[���擾
    D3D12_INDEX_BUFFER_VIEW GetView() const;

private:
    ComPtr<ID3D12Resource>  m_pIB;      // �C���f�b�N�X�o�b�t�@�[
    D3D12_INDEX_BUFFER_VIEW m_View;     // �C���f�b�N�X�o�b�t�@�[�r���[

    IndexBuffer(const IndexBuffer&) = delete;
    void operator = (const IndexBuffer&) = delete;
};