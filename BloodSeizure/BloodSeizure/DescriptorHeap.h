#pragma once
#include "GraphicsEngine.h"

/// <summary>
/// �f�B�X�N���v�^�q�[�v
/// </summary>
class DescriptorHeap
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    DescriptorHeap();
    
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~DescriptorHeap();

    ID3D12DescriptorHeap* Get() const;


};