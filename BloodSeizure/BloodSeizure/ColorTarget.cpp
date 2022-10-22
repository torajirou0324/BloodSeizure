#include "ColorTarget.h"
#include "DescriptorPool.h"

// コンストラクタ
ColorTarget::ColorTarget()
    : m_pTarget(nullptr)
    , m_pHandleRTV(nullptr)
    , m_pPoolRTV(nullptr)
{
}

// デストラクタ
ColorTarget::~ColorTarget()
{
    Term();
}

// 初期化処理
bool ColorTarget::Init(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t width, uint32_t height, DXGI_FORMAT format)
{
    // 引数チェック
    if (pDevice == nullptr || pPoolRTV == nullptr || width == 0 || height == 0)
    {
        return false;
    }

    assert(m_pHandleRTV == nullptr);
    assert(m_pPoolRTV == nullptr);

    m_pHandleRTV = m_pPoolRTV->AllocHandle();
    if (m_pHandleRTV == nullptr)
    {
        return false;
    }

    // プロパティ設定
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_DEFAULT;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;


    return false;
}

// バックバッファから初期化を開始する
bool ColorTarget::InitFromBackBuffer(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t index, IDXGISwapChain* pSwapChain)
{
    return false;
}

// 解放処理
void ColorTarget::Term()
{
}

// ディスクリプタハンドル（RTV用）を取得
DescriptorHandle* ColorTarget::GetHandleRTV() const
{
    return nullptr;
}

// リソースを取得
ID3D12Resource* ColorTarget::GetResource() const
{
    return nullptr;
}

// リソースの構造体を取得
D3D12_RESOURCE_DESC ColorTarget::GetDesc() const
{
    return D3D12_RESOURCE_DESC();
}

// ビュー構造体を取得
D3D12_RENDER_TARGET_VIEW_DESC ColorTarget::GetViewDesc() const
{
    return D3D12_RENDER_TARGET_VIEW_DESC();
}
