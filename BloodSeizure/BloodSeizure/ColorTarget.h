#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdint>
#include "ComPtr.h"

class DescriptorHandle;
class DescriptorPool;

class ColorTarget
{
public:
    ColorTarget();
    ~ColorTarget();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t width, uint32_t height, DXGI_FORMAT format);

    // バックバッファから初期化処理を実行
    bool InitFromBackBuffer(ID3D12Device* pDevice, DescriptorPool* pPoolRTV, uint32_t index, IDXGISwapChain* pSwapChain);

    // 解放処理
    void Term();

    // ディスクリプタハンドル（RTV用）を取得
    DescriptorHandle* GetHandleRTV() const;

    // リソースを取得
    ID3D12Resource* GetResource() const;

    // リソース設定を取得
    D3D12_RESOURCE_DESC GetDesc() const;

    // レンダーターゲットビューの設定を取得
    D3D12_RENDER_TARGET_VIEW_DESC GetViewDesc() const;

private:
    ComPtr<ID3D12Resource>          m_pTarget;      // リソース
    DescriptorHandle*               m_pHandleRTV;   // ディスクリプタハンドル（RTV用）
    DescriptorPool*                 m_pPoolRTV;     // ディスクリプタプール（RTV用）
    D3D12_RENDER_TARGET_VIEW_DESC   m_ViewDesc;     // レンダーターゲットビュー構造体

    ColorTarget(const ColorTarget&) = delete;
    void operator = (const ColorTarget&) = delete;
};