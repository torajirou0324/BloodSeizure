#pragma once

#include "ResMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    // ����������
    bool Init(ID3D12Device* pDevice, const ResMesh& resource);

    // �I������
    void Term();

    // �`�揈��
    void Draw(ID3D12GraphicsCommandList* pCmdList);

    // �}�e���A��ID���擾���܂�
    uint32_t GetMaterialId() const;

private:
    VertexBuffer    m_VB;               // ���_�o�b�t�@
    IndexBuffer     m_IB;               // �C���f�b�N�X�o�b�t�@
    uint32_t        m_MaterialId;       // �}�e���A��ID
    uint32_t        m_IndexCount;       // �C���f�b�N�X��

    Mesh            (const Mesh&) = delete;
    void operator = (const Mesh&) = delete;
};