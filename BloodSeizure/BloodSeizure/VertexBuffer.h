#pragma once

#include <d3d12.h>
#include "ComPtr.h"

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    // ����������
    bool Init(ID3D12Device* pDevice, size_t size, size_t stride, const void* pInitData = nullptr);

    // ����������
    template<typename T>
    bool Init(ID3D12Device* pDevice,size_t size,const T* pInitData = nullptr)
    {
        return Init(pDevice, size, sizeof(T), pInitData);
    }

    // �I������
    void Term();

    // �������}�b�s���O
    void* Map();

    // �������}�b�s���O������
    void Unmap();

    // �������}�b�s���O
    template<typename T>
    T* Map() const
    {
        return reinterpret_cast<T*>(Map());
    }

    // ���_�o�b�t�@�r���[���擾
    D3D12_VERTEX_BUFFER_VIEW GetView() const;

private:
    ComPtr<ID3D12Resource>      m_pVB;      // ���_�o�b�t�@
    D3D12_VERTEX_BUFFER_VIEW    m_View;     // ���_�o�b�t�@�r���[

    VertexBuffer(const VertexBuffer&) = delete;
    void operator = (const VertexBuffer&) = delete;
};