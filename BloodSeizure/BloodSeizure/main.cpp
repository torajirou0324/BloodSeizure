#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "system.h"
#include "RootSignature.h"
#include "Shader.h"
#include "PipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化処理
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	RootSignature rootSignature;
	D3D12_ROOT_PARAMETER param = {};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	param.Descriptor.ShaderRegister = 0;
	param.Descriptor.RegisterSpace = 0;
	param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootSignature.Init(&param, 0);

	Shader vs, ps;
	vs.LoadVS(L"LambertVS.cso");
	ps.LoadPS(L"LambertPS.cso");

	// 頂点レイアウトを定義する
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	PipelineState pipelineState;
	pipelineState.Init(inputElementDescs, 2, rootSignature.Get(), vs, ps);

	Vertex vertices[] = {
		{DirectX::XMFLOAT3(-1.0f,-1.0f,0.0f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(1.0f,-1.0f,0.0f), DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(0.0f,1.0f,0.0f), DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},
	};

	VertexBuffer triangleVB;
	triangleVB.Init(sizeof(vertices), sizeof(vertices[0]));
	triangleVB.Copy(vertices);

	uint16_t indices[] = {
		0,1,2
	};
	IndexBuffer triangleIB;
	triangleIB.Init(sizeof(indices), 2);
	triangleIB.Copy(indices);

	auto commandList = g_graphicsEngine->GetCommandList();

	// ゲームループ
	while (DispatchWindowMessage())
	{
		// 1フレームの開始（シーンのクリア）
		g_graphicsEngine->BeginRender();

		//auto root = rootSignature.Get();
		commandList->SetGraphicsRootSignature(rootSignature.Get());

		//auto pipe = pipelineState.Get();
		commandList->SetPipelineState(pipelineState.Get());

		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto vbdesc = triangleVB.GetView();
		commandList->IASetVertexBuffers(0, 1, &vbdesc);

		auto ibdesc = triangleIB.GetView();
		commandList->IASetIndexBuffer(&ibdesc);

		commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

		// 1フレームの終了（表画面と裏画面を入れ替える）
		g_graphicsEngine->EndRender();
	}

	// ゲームの終了処理
	TermGame();

	return 0;
}