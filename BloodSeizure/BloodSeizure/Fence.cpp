#include "Fence.h"

// �R���X�g���N�^
Fence::Fence()
    : m_pFence(nullptr)
    , m_Event(nullptr)
    , m_Counter(0)
{
}

// �f�X�g���N�^
Fence::~Fence()
{
    Term();
}

// ����������
bool Fence::Init(ID3D12Device* pDevice)
{
    // �����`�F�b�N
    if (pDevice == nullptr)
    {
        return false;
    }

    // �C�x���g�𐶐�
    m_Event = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
    if (m_Event == nullptr)
    {
        return false;
    }

    // �t�F���X���쐬
    auto hr = pDevice->CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(m_pFence.GetAddressOf())
    );
    if (FAILED(hr))
    {
        return false;
    }

    // �J�E���^��ݒ�
    m_Counter = 1;

    return true;
}

// �������
void Fence::Term()
{
    // �n���h�������
    if (m_Event != nullptr)
    {
        CloseHandle(m_Event);
        m_Event = nullptr;
    }

    // �t�F���X�I�u�W�F�N�g�j��
    m_pFence.Reset();

    // �J�E���^�����Z�b�g
    m_Counter = 0;
}

// �V�O�i����ԂɂȂ�܂Ŏw�莞�ԑҋ@
void Fence::Wait(ID3D12CommandQueue* pQueue, UINT timeout)
{
    if (pQueue == nullptr)
    {
        return;
    }

    const auto fenceValue = m_Counter;

    // �V�O�i������
    auto hr = pQueue->Signal(m_pFence.Get(), fenceValue);
    if (FAILED(hr))
    {
        return;
    }

    // �J�E���^�𑝂₷
    m_Counter++;

    // ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
    if (m_pFence->GetCompletedValue() < fenceValue)
    {
        // �������ɃC�x���g��ݒ�
        auto hr = m_pFence->SetEventOnCompletion(fenceValue, m_Event);
        if (FAILED(hr))
        {
            return;
        }

        // �ҋ@����
        if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, timeout, FALSE))
        {
            return;
        }
    }
}

// �V�O�i����ԂɂȂ�܂ł����Ƒҋ@
void Fence::Sync(ID3D12CommandQueue* pQueue)
{
    if (pQueue == nullptr)
    {
        return;
    }

    // �V�O�i������
    auto hr = pQueue->Signal(m_pFence.Get(), m_Counter);
    if (FAILED(hr))
    {
        return;
    }

    // �������ɃC�x���g��ݒ�
    hr = m_pFence->SetEventOnCompletion(m_Counter, m_Event);
    if (FAILED(hr))
    {
        return;
    }

    // �ҋ@����
    if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, INFINITE, FALSE))
    {
        return;
    }

    // �J�E���^�𑝂₷
    m_Counter;
}
