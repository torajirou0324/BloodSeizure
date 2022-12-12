#include "Shader.h"
#include <string>
#include "FileUtil.h"

namespace {
	const char* g_vsShaderModelName = "vs_5_0";	//頂点シェーダーのシェーダーモデル名。
	const char* g_psShaderModelName = "ps_5_0";	//ピクセルシェーダーのシェーダモデル名。
	const char* g_csShaderModelName = "cs_5_0";	//コンピュートシェーダーのシェーダーモデル名。
}

void Shader::Load(const char* filePath, const char* entryFuncName, const char* shaderModel)
{


	m_isInited = true;
}

void Shader::LoadPS(const wchar_t* filePath)
{
	std::wstring psPath;
	SearchFilePath(filePath, psPath);

	D3DReadFileToBlob(psPath.c_str(), &m_blob);
}

void Shader::LoadVS(const wchar_t* filePath)
{
	std::wstring vsPath;
	SearchFilePath(filePath, vsPath);

	D3DReadFileToBlob(vsPath.c_str(), &m_blob);
}
