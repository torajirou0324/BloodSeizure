#pragma once
#include "GraphicsEngine.h"
#include "DirectXHelpers.h"
#include "SimpleMath.h"

// �Q�[���̏�����
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���A���Ă�����Q�[���I��
bool DispatchWindowMessage();

const UINT FRAME_BUFFER_W = 1920;   // ��ʂ̉���
const UINT FRAME_BUFFER_H = 1080;   // ��ʂ̏c��

// GPU�Ŏg�p����s����܂Ƃ߂��\����
struct Transform
{
    DirectX::SimpleMath::Matrix World;          // ���[���h�s��
    DirectX::SimpleMath::Matrix View;           // �r���[�s��
    DirectX::SimpleMath::Matrix Proj;           // �v���W�F�N�V�����s��
};

struct LightBuffer
{
    DirectX::SimpleMath::Vector4 LightPosition; // ���C�g�ʒu
    DirectX::SimpleMath::Color LightColor;      // ���C�g�J���[
};

struct MaterialBuffer
{
    DirectX::SimpleMath::Vector3 Diffuse;       // �g�U���˗�
    float Alpha;                                // ���ߓx
};