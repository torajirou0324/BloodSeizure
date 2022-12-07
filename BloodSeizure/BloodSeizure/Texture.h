#pragma once

#include <d3d12.h>
#include <ResourceUploadBatch.h>
#include "ComPtr.h"

class Texture
{
public:
    Texture();
    ~Texture();

    // 初期化処理(batch)テクスチャの更新に必要なデータを格納する
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, const wchar_t* filename, DirectX::ResourceUploadBatch& batch);

    // 初期化処理(isCube)キューブマップである場合はtrueを指定
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, const D3D12_RESOURCE_DESC* pDesc, bool isCube);

    // 解放処理
    void Term();

    // CPUディスクリプタハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU() const;

    // GPUディスクリプタハンドルを取得
    D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU() const;

private:
    ComPtr<ID3D12Resource>  m_pTex;
    DescriptorHandle*       m_pHandle;
    DescriptorPool*         m_pPool;

    Texture(const Texture&) = delete;
    void operator = (const Texture&) = delete;

    // シェーダーリソースの設定を取得
    D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool isCube);
};