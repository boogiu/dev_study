#include "Shader.h"
#include "Helper_Func.h"
CShader::CShader()
{
}

CShader::~CShader()
{
}

HRESULT CShader::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Shader_Desc* pArg)
{
	//HRESULT D3DX11CompileEffectFromFile(
	//	LPCWSTR pFileName,          // [입력] 컴파일할 .fx 파일 경로 (wstring)
	//	const D3D_SHADER_MACRO * pDefines, // [입력] 셰이더 매크로 정의 (없으면 NULL)
	//	ID3DInclude * pInclude,        // [입력] #include 처리기 (기본값은 NULL)
	//	UINT HLSLFlags,             // [입력] HLSL 컴파일러 플래그
	//	UINT FXFlags,               // [입력] 이펙트 컴파일러 플래그 (보통 0)
	//	ID3D11Device * pDevice,        // [입력] D3D11 디바이스
	//	ID3DBlob * *ppEffect,          // [출력] 성공 시 컴파일된 바이트코드를 받을 Blob
	//	ID3DBlob * *ppErrorMsgs        // [출력] 실패 시 에러 메시지를 받을 Blob
	//); 
	_uint		iShaderFlag = {};

#ifdef _DEBUG
	iShaderFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iShaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	wstring wPath = Helper::ConvertToWideString(pArg->ShaderPath);

	HRESULT CompileHr = D3DX11CompileEffectFromFile(wPath.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, iShaderFlag, 0, pDevice, &m_pEffect, nullptr);

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc{};

	pTechnique->GetDesc(&TechniqueDesc);

	m_iPassCount = TechniqueDesc.Passes;

	m_InputLayouts.reserve(m_iPassCount);

	for (size_t i = 0; i < TechniqueDesc.Passes; i++)
	{
		ID3D11InputLayout* pInputLayout = { nullptr };
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(static_cast<uint32_t>(i));

		D3DX11_PASS_DESC		PassDesc{};

		pPass->GetDesc(&PassDesc);

		if (FAILED(pDevice->CreateInputLayout(pArg->pElements,2 ,
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}

	return CompileHr;
}


CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Shader_Desc* pArg)
{
	if(pArg ==nullptr)
		return nullptr;

	CShader* instance = new CShader();
	if (FAILED(instance->Initialize(pDevice, pContext, pArg))) {
		Safe_Release(instance);
	}

	return instance;
}

void CShader::Free()
{
}
