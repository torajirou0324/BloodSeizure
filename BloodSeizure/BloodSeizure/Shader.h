#pragma once
#include "GraphicsEngine.h"

class Shader
{
public:
    /// <summary>
    /// �s�N�Z���V�F�[�_�����[�h
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X�B</param>
    /// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
    void LoadPS(const wchar_t* filePath);

    /// <summary>
    /// ���_�V�F�[�_�����[�h
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X�B</param>
    /// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
    void LoadVS(const wchar_t* filePath);

    /// <summary>
    /// �R���p�C���ςݒ��_�V�F�[�_�[�̃f�[�^���擾
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledBlob() const
    {
        return m_blob;
    }

    /// <summary>
    /// �������ς݂��ǂ����̃t���O���擾
    /// </summary>
    /// <returns></returns>
    bool IsInited() const
    {
        return m_isInited;
    }

private:
    /// <summary>
    /// �V�F�[�_�[�����[�h�B
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X</param>
    /// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
    /// <param name="shaderModel">�V�F�[�_�[���f��</param>
    void Load(const char* filePath, const char* entryFuncName, const char* shaderModel);

    ID3DBlob* m_blob = nullptr;   // �R���p�C���ς݂̒��_�V�F�[�_�[�̃f�[�^
    bool m_isInited = false;        // �������ς݂��ǂ���
};