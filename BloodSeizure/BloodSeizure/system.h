#pragma once
#include "GraphicsEngine.h"

// �Q�[���̏�����
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���A���Ă�����Q�[���I��
bool DispatchWindowMessage();

const UINT FRAME_BUFFER_W = 1920;   // ��ʂ̉���
const UINT FRAME_BUFFER_H = 1080;   // ��ʂ̏c��