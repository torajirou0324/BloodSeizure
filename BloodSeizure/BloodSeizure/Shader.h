#pragma once

class Shader
{
public:
    /// <summary>
    /// �s�N�Z���V�F�[�_�����[�h
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X�B</param>
    /// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
    void LoadPS(const char* filePath, const char* entryFuncName);

    /// <summary>
    /// ���_�V�F�[�_�����[�h
    /// </summary>
    /// <param name="filePath">�t�@�C���p�X�B</param>
    /// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
    void LoadVS(const char* filePath, const char* entryFuncName);

    /// <summary>
    /// �R���p�C���ςݒ��_�V�F�[�_�[�̃f�[�^���擾
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledVSBlob() const
    {
        return m_vsBlob;
    }

    /// <summary>
    /// �R���p�C���ς݂̃s�N�Z���V�F�[�_�[�̃f�[�^���擾
    /// </summary>
    /// <returns></returns>
    ID3DBlob* GetCompiledPSBlob() const
    {
        return m_psBlob;
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
    ID3DBlob* m_vsBlob = nullptr;   // �R���p�C���ς݂̒��_�V�F�[�_�[�̃f�[�^
    ID3DBlob* m_psBlob = nullptr;   // �R���p�C���ς݂̃s�N�Z���V�F�[�_�[�̃f�[�^
    bool m_isInited = false;        // �������ς݂��ǂ���
};