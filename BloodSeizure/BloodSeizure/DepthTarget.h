#pragma once

#include <d3d12.h>
#include <cstdint>
#include "ComPtr.h"

class DescriptorHandle;
class DescriptorPool;

class DepthTarget
{
public:
    DepthTarget();
    ~DepthTarget();

    // 初期化処理
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPoolDSV, uint32_t width, uint32_t height, DXGI_FORMAT format);

    // 解放処理
    void Term();

    // ディスクリプタハンドル（DSV用）を取得
    DescriptorHandle* GetHandleDSV() const;

    // リソースを取得
    ID3D12Resource* GetResource() const;

    // リソース設定を取得
    D3D12_RESOURCE_DESC GetDesc() const;

    // 深度ステンシルビューの設定を取得
    D3D12_DEPTH_STENCIL_VIEW_DESC GetViewDesc() const;

private:
    ComPtr<ID3D12Resource>          m_pTarget;      // リソース
    DescriptorHandle*               m_pHandleDSV;   // ディスクリプタハンドル
    DescriptorPool*                 m_pPoolDSV;     // ディスクリプタプール
    D3D12_DEPTH_STENCIL_VIEW_DESC   m_ViewDesc;     // 深度ステンシルビューの設定

    DepthTarget(const DepthTarget&) = delete;
    void operator = (const DepthTarget&) = delete;
};