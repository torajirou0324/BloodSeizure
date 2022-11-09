#include "Mesh.h"
#include <d3d12.h>

// �R���X�g���N�^
Mesh::Mesh()
    : m_MaterialId(UINT32_MAX)
    , m_IndexCount(0)
{
}

// �f�X�g���N�^
Mesh::~Mesh()
{
    Term();
}

// ����������
bool Mesh::Init(ID3D12Device* pDevice, const ResMesh& resource)
{
    if (pDevice == nullptr)
    {
        return false;
    }

    if (!m_VB.Init(
        pDevice,
        sizeof(MeshVertex) * resource.Vertices.size(),
        resource.Vertices.data()
    ))
    {
        false;
    }

    if (!m_IB.Init(
        pDevice,
        sizeof(uint32_t) * resource.Indices.size(),
        resource.Indices.data()
    ))
    {
        return false;
    }

    return true;
}

// �I������
void Mesh::Term()
{
    m_VB.Term();
    m_IB.Term();
    m_MaterialId = UINT32_MAX;
    m_IndexCount = 0;
}

// �`�揈��
void Mesh::Draw(ID3D12GraphicsCommandList* pCmdList)
{
    auto VBV = m_VB.GetView();
    auto IBV = m_IB.GetView();
    pCmdList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pCmdList->IASetVertexBuffers(0, 1, &VBV);
    pCmdList->IASetIndexBuffer(&IBV);
    pCmdList->DrawIndexedInstanced(m_IndexCount, 1, 0, 0, 0);
}

// �}�e���A��ID���擾
uint32_t Mesh::GetMaterialId() const
{
    return m_MaterialId;
}
