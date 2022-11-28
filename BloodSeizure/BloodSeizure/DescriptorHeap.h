#pragma once
#include "GraphicsEngine.h"

/// <summary>
/// ディスクリプタヒープ
/// </summary>
class DescriptorHeap
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    DescriptorHeap();
    
    /// <summary>
    /// デストラクタ
    /// </summary>
    ~DescriptorHeap();

    ID3D12DescriptorHeap* Get() const;


};