#include <stdio.h>
#include <stdlib.h>
#include "system.h"

// ���_�\����
struct SimpleVertex
{
	float position[3];
	float color[4];
};

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏���������
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	RootSignature rootSignature;
	D3D12_ROOT_PARAMETER param = {};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	param.Descriptor.ShaderRegister = 0;
	param.Descriptor.RegisterSpace = 0;
	param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootSignature.Init(&param, 0);

	Shader vs, ps;
	vs.LoadVS("Lambert.cso", "main");
	ps.LoadPS("Lambert.cso", "main");

	PipelineState pipelineState;
	// ���_���C�A�E�g���`����
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	//pipelineState.Init()

	// �Q�[�����[�v
	while (DispatchWindowMessage())
	{
		// 1�t���[���̊J�n�i�V�[���̃N���A�j
		g_graphicsEngine->BeginRender();



		// 1�t���[���̏I���i�\��ʂƗ���ʂ����ւ���j
		g_graphicsEngine->EndRender();
	}

	// �Q�[���̏I������
	TermGame();

	return 0;
}