#pragma once

#include <ResourceUploadBatch.h>
#include <map>
#include "DescriptorPool.h"
#include "Texture.h"
#include "ConstantBuffer.h"

class Material
{
public:
    // �e�N�X�`���̐ݒ�
    enum TEXTURE_USAGE
    {
        TEXTURE_USAGE_DIFFUSE = 0,  // �f�B�t���[�Y�}�b�v�Ƃ��ė��p
        TEXTURE_USAGE_SPECULAR,     // �X�y�L�����}�b�v�Ƃ��ė��p
        TEXTURE_USAGE_SHININESS,    // �V���C�l�X�}�b�v�Ƃ��ė��p
        TEXTURE_USAGE_NORMAL,       // �@���}�b�v�Ƃ��ė��p

        TEXTURE_USAGE_COUNT
    };

    Material();
    ~Material();

    // ����������
    bool Init(ID3D12Device* pDevice, DescriptorPool* pPool, size_t bufferSize, size_t count);

    // �������
    void Term();

    // �e�N�X�`���ݒ�
    bool SetTexture(size_t index, TEXTURE_USAGE usage, const std::wstring& path, DirectX::ResourceUploadBatch& batch);

    // �萔�o�b�t�@�̃|�C���^���擾
    void* GetBufferPtr(size_t index) const;

    // �萔�o�b�t�@�̃|�C���^���擾
    template<typename T>
    T* GetBufferPtr(size_t index) const
    {
        return reinterpret_cast<T*>(GetBufferPtr(index));
    }

    // �萔�o�b�t�@��GPU���z�A�h���X���擾
    D3D12_GPU_VIRTUAL_ADDRESS GetBufferAddress(size_t index) const;

    // �e�N�X�`���n���h�����擾
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(size_t index, TEXTURE_USAGE usage) const;

    // �}�e���A�������擾
    size_t GetCount() const;

private:
    struct Subset
    {
        ConstantBuffer*             pConstantBuffer;                    // �萔�o�b�t�@
        D3D12_GPU_DESCRIPTOR_HANDLE TextureHandle[TEXTURE_USAGE_COUNT]; // �e�N�X�`���n���h��
    };

    std::map<std::wstring, Texture*>    m_pTexture;     // �e�N�X�`���Z�b�g
    std::vector<Subset>                 m_Subset;       // �T�u�Z�b�g
    ID3D12Device*                       m_pDevice;      // �f�o�C�X
    DescriptorPool*                     m_pPool;        // �f�B�X�N���v�^�q�[�v(CBV_UAV_SRV)

    Material(const Material&) = delete;
    void operator = (const Material&) = delete;
};

constexpr auto TU_DIFFUSE   = Material::TEXTURE_USAGE_DIFFUSE;
constexpr auto TU_SPECULAR  = Material::TEXTURE_USAGE_SPECULAR;
constexpr auto TU_SHININESS = Material::TEXTURE_USAGE_SHININESS;
constexpr auto TU_NORMAL    = Material::TEXTURE_USAGE_NORMAL;