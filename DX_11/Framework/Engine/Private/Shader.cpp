#include "Shader.h"
#include "Helper_Func.h"
CShader::CShader()
{
}

CShader::~CShader()
{
}

HRESULT CShader::Initialize(ID3D11Device* pDevice, const string& filePath)
{
	_uint		iCompileFlag = {};

#ifdef _DEBUG
	iCompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iShaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	wstring wPath = Helper::ConvertToWideString(filePath);

	HRESULT CompileHr = D3DX11CompileEffectFromFile(wPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, iCompileFlag, 0,
		pDevice, &m_pEffect, nullptr);

	if (FAILED(CompileHr))
		return E_FAIL;

	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	if (nullptr == m_pTechnique)
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::GetPassSignature(UINT iPassIndex, D3DX11_PASS_DESC* pOutPassDesc)
{
	if (!m_pTechnique || !pOutPassDesc)
		return E_FAIL;

	ID3DX11EffectPass* pPass = m_pTechnique->GetPassByIndex(iPassIndex);

	if (!pPass)
		return E_FAIL;
	
	pPass->GetDesc(pOutPassDesc);
	return S_OK;
}


CShader* CShader::Create(ID3D11Device* pDevice, const string& filePath)
{
	CShader* instance = new CShader();
	if (FAILED(instance->Initialize(pDevice, filePath))) {
		MessageBoxA(nullptr, filePath.c_str(), "CShader Create", MB_OK);
		Safe_Release(instance);
	}

	return instance;
}

void CShader::Free()
{

}
