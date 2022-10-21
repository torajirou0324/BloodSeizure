#pragma once

#include <cstdint>
#include <mutex>
#include <cassert>
#include <functional>

template<typename T>
class Pool
{
public:
    // �R���X�g���N�^
    Pool()
        : m_pBuffer(nullptr)
        , m_pActive(nullptr)
        , m_pFree(nullptr)
        , m_Capacity(0)
        , m_Count(0)
    {}

    // �f�X�g���N�^
    ~Pool()
    {
        Term();
    }

    // ����������
    bool Init(uint32_t count)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        m_pBuffer = static_cast<uint8_t*>(malloc(sizeof(Item) * (count + 2)));
        if (m_pBuffer == nullptr)
        {
            return false;
        }

        m_Capacity = count;

        // �C���f�b�N�X��U��
        for (auto i = 2u, j = 0u; i < m_Capacity + 2; ++i, ++j)
        {
            auto item = GetItem(i);
            item->m_Index = j;
        }

        m_pActive = GetItem(0);
        m_pActive->m_pPrev = m_pActive->m_pNext = m_pActive;
        m_pActive->m_Index = uint32_t(-1);

        m_pFree = GetItem(1);
        m_pFree->m_Index = uint32_t(-2);

        for (auto i = 1u; i < m_Capacity + 2; ++i)
        {
            GetItem(i)->m_pPrev = nullptr;
            GetItem(i)->m_pNext = GetItem(i + 1);
        }

        GetItem(m_Capacity + 1)->m_pPrev = m_pFree;

        m_Count = 0;

        return true;
    }

    // �I������
    void Term()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        if (m_pBuffer)
        {
            free(m_pBuffer);
            m_pBuffer = nullptr;
        }

        m_pActive = nullptr;
        m_pFree = nullptr;
        m_Capacity = 0;
        m_Count = 0;
    }

    // �A�C�e�����m��
    T* Alloc(std::function<void(uint32_t, T*)> func = nullptr)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        if (m_pFree->m_pNext == m_pFree || m_Count + 1 > m_Capacity)
        {
            return nullptr;
        }

        auto item = m_pFree->m_pNext;
        m_pFree->m_pNext = item->m_pNext;

        item->m_pPrev = m_pActive->m_pPrev;
        item->m_pNext = m_pActive;
        item->m_pPrev->m_pNext = item->m_pNext->m_pPrev = item;

        m_Count++;

        // ���������蓖��.
        auto val = new ((void*)item) T();

        // �������̕K�v������ΌĂяo��.
        if (func != nullptr)
        {
            func(item->m_Index, val);
        }

        return val;
    }

    // �A�C�e�������
    void Free(T* pValue)
    {
        if (pValue == nullptr)
        {
            return;
        }

        std::lock_guard<std::mutex> guard(m_Mutex);

        auto item = reinterpret_cast<Item*>(pValue);

        item->m_pPrev->m_pNext = item->m_pNext;
        item->m_pNext->m_pPrev = item->m_pPrev;

        item->m_pPrev = nullptr;
        item->m_pNext = m_pFree->m_pNext;

        m_pFree->m_pNext = item;
        m_Count--;
    }

    // ���A�C�e�������擾
    uint32_t GetSize() const
    {
        return m_Capacity;
    }

    // �g�p���̃A�C�e�������擾
    uint32_t GetUsedCount() const
    {
        return m_Count;
    }

    // ���p�\�ȃA�C�e�������擾
    uint32_t GetAvailableCount() const
    {
        return m_Capacity - m_Count;
    }

private:
    // �v���C�x�[�g�\����
    struct Item
    {
        T           m_Value;    // �l
        uint32_t    m_Index;    // �C���f�b�N�X
        Item*       m_pNext;    // ���̃A�C�e���ւ̃|�C���^
        Item*       m_pPrev;    // �O�̃A�C�e���ւ̃|�C���^

        Item()
            : m_Value()
            , m_Index(0)
            , m_pNext(nullptr)
            , m_pPrev(nullptr)
        {}

        ~Item()
        {}
    };

    // �v���C�x�[�g�ϐ�
    uint8_t*    m_pBuffer;      // �o�b�t�@�ł�
    Item*       m_pActive;      // �A�N�e�B�u�A�C�e���̐擪
    Item*       m_pFree;        // �t���[�A�C�e���̐擪
    uint32_t    m_Capacity;     // ���A�C�e����
    uint32_t    m_Count;        // �m�ۂ����A�C�e����
    std::mutex  m_Mutex;        // �~���[�e�b�N�X

    // �v���C�x�[�g�֐�

    // �A�C�e�����擾
    Item* GetItem(uint32_t index)
    {
        assert(0 <= index && index <= m_Capacity + 2);
        return reinterpret_cast<Item*>(m_pBuffer + sizeof(Item) * index);
    }

    // �A�C�e���Ƀ����������蓖�Ă�
    Item* AssignItem(uint32_t index)
    {
        assert(0 <= index && index <= m_Capacity + 2);
        auto buf = (m_pBuffer + sizeof(Item) * index);
        return new (buf) Item;
    }

    Pool(const Pool&) = delete;
    void operator = (const Pool&) = delete;
};