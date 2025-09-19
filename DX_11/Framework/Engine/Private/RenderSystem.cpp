#include "RenderSystem.h"
#include "Model.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "ICameraService.h"
#include "Shader.h"
#include "PipeLine.h"
#include"Material.h"

CRenderSystem::CRenderSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice{pDevice},m_pContext{ pContext }
{
	Safe_AddRef(pDevice);
	Safe_AddRef(m_pContext);
}

CRenderSystem::~CRenderSystem()
{
}

HRESULT CRenderSystem::Initialize()
{
	/*pipeLine*/
	m_pPipeLine = CPipeLine::Create(m_pDevice);

	/*RenderPass*/
	m_pOpaquePass = OpaquePass::Create(this);
	m_pUIPass = UIPass::Create(this);
	return S_OK;
}

HRESULT CRenderSystem::Render()
{
	m_pPipeLine->Update_FrameBuffer(m_pContext);
	m_pPipeLine->Update_LightBuffer(m_pContext);

	m_pOpaquePass->Execute(m_pContext);
	m_pUIPass->Execute(m_pContext);
	return S_OK;
}

CRenderSystem* CRenderSystem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderSystem* instance = new CRenderSystem(pDevice, pContext);
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

HRESULT CRenderSystem::Get_InputLayout(CModel* pModel, CShader* pShader, _uint DrawIndex, const string& passConstant,ID3D11InputLayout** ppInputLayout)
{
	if (!pModel || !pShader || !ppInputLayout)
		return E_FAIL;
	
	/*모델 데이터 이름 + 셰이더 이름*/
	string LayOutID = string(pModel->Get_ElementKey(DrawIndex)) + pShader->Get_Key();

	auto iter = m_InputLayouts.find(LayOutID);
	
	if (iter != m_InputLayouts.end()) {
		*ppInputLayout = iter->second;
		return S_OK;
	}

	D3DX11_PASS_DESC passDesc = {};
	
	if (FAILED(pShader->GetPassSignature(passConstant, &passDesc)))
		return E_FAIL;

	if (pModel->Get_ElementCount(DrawIndex) == 0 || pModel->Get_ElementDesc(DrawIndex) == nullptr)
		return E_FAIL;
	
	HRESULT hr = m_pDevice->CreateInputLayout(
		pModel->Get_ElementDesc(DrawIndex), pModel->Get_ElementCount(DrawIndex),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		ppInputLayout);
	
	if (FAILED(hr))
		return E_FAIL;
	
	m_InputLayouts.emplace(LayOutID, *ppInputLayout);

	return S_OK;
}

void CRenderSystem::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pPipeLine);

	Safe_Release(m_pOpaquePass);
	Safe_Release(m_pUIPass);
	
	for (auto& pair : m_InputLayouts)
		Safe_Release(pair.second);

	m_InputLayouts.clear();
}
