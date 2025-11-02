#include "RenderSystem.h"
#include "Model.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "ICameraService.h"
#include "Shader.h"
#include "PipeLine.h"
#include"Material.h"
#include "Target_Manager.h"
#include "IResourceService.h"
#include "VIBuffer.h"
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
	m_pTargetManager = CTarget_Manager::Create(m_pDevice,m_pContext);
	
	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_pTargetManager->Add_RenderTarget("Target_Diffuse", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));
	m_pTargetManager->Add_RenderTarget("Target_Normal", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));
	m_pTargetManager->Add_RenderTarget("Target_Shade", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));

	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_LightAcc", "Target_Shade")))
		return E_FAIL;

	m_pShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(G_GlobalLevelKey,"Shader_Deferred.hlsl");
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pVIBuffer = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_VIBuffer(G_GlobalLevelKey, "Engine_Default_Rect",BUFFER_TYPE::BASIC_RECT);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));

	/*RenderPass*/
	m_pOpaquePass = OpaquePass::Create(this);
	m_pInstancePass = InstancePass::Create(this);
	m_pUIPass = UIPass::Create(this);

#ifdef _DEBUG
	m_pDebugPass = DebugPass::Create(this);
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderSystem::Render()
{
	m_pPipeLine->Update_FrameBuffer(m_pContext);
	m_pPipeLine->Update_LightBuffer(m_pContext);

	if (FAILED(m_pTargetManager->Begin_MRT("MRT_Deferred"))) return E_FAIL;
		m_pOpaquePass->Execute(m_pContext);
		m_pInstancePass->Execute(m_pContext);
	if (FAILED(m_pTargetManager->End_MRT()))return E_FAIL;


	m_pShader->SetConstantBuffer("LightBuffer", m_pPipeLine->Get_LightBuffer());
	Render_LightAcc();
	Render_Combined();


#ifdef _DEBUG
	m_pDebugPass->Execute(m_pContext);
#endif // _DEBUG

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

HRESULT CRenderSystem::Render_LightAcc()
{
	if (FAILED(m_pTargetManager->Begin_MRT("MRT_LightAcc"))) return E_FAIL;

	ID3D11InputLayout* pLayout;
	Get_BufferInputLayout(m_pVIBuffer, m_pShader, "Combined", &pLayout);
	m_pContext->IASetInputLayout(pLayout);

	SHADER_PARAM NoramlParam = {};
	m_pTargetManager->Get_TargetParam("Target_Normal", NoramlParam);
	m_pShader->Bind_Value("g_NormalTexture", NoramlParam);

	SHADER_PARAM WorldMat = {};
	WorldMat.iSize = sizeof(_float4x4);
	WorldMat.typeName = "float4x4";
	WorldMat.pData = &m_WorldMatrix;
	m_pShader->Bind_Value("g_WorldMatrix", WorldMat);

	m_pShader->Apply("Directional", m_pContext);
	m_pVIBuffer->Bind_Buffer(m_pContext);
	m_pVIBuffer->Render(m_pContext);

	if (FAILED(m_pTargetManager->End_MRT()))return E_FAIL;

	return S_OK;
}

HRESULT CRenderSystem::Render_Combined()
{
	m_pShader->SetConstantBuffer("FrameBuffer", m_pPipeLine->Get_FrameBuffer());

	ID3D11InputLayout* pLayout;
	Get_BufferInputLayout(m_pVIBuffer, m_pShader, "Combined", &pLayout);
	m_pContext->IASetInputLayout(pLayout);

	SHADER_PARAM NoramlParam = {};
	m_pTargetManager->Get_TargetParam("Target_Diffuse", NoramlParam);
	m_pShader->Bind_Value("g_DiffuseTexture", NoramlParam);

	SHADER_PARAM ShadeParam = {};
	m_pTargetManager->Get_TargetParam("Target_Shade", ShadeParam);
	m_pShader->Bind_Value("g_ShadeTexture", ShadeParam);

	SHADER_PARAM WorldMat = {};
	WorldMat.iSize = sizeof(_float4x4);
	WorldMat.typeName = "float4x4";
	WorldMat.pData = &m_WorldMatrix;
	m_pShader->Bind_Value("g_WorldMatrix", WorldMat);

	m_pShader->Apply("Combined", m_pContext);
	m_pVIBuffer->Bind_Buffer(m_pContext);
	m_pVIBuffer->Render(m_pContext);
	return S_OK;
}

#ifdef _USING_GUI
void CRenderSystem::Render_GUI()
{
	m_pTargetManager->Render_GUI();
}
#endif // _USING_GUI

HRESULT CRenderSystem::Get_InputLayout(CModel* pModel, CShader* pShader, _uint DrawIndex, const string& passConstant,ID3D11InputLayout** ppInputLayout)
{
	if (!pModel || !pShader || !ppInputLayout)
		return E_FAIL;

	/*메쉬를 생성할 떄마다 레이아웃을 만드는게아닌, 버텍스구조, 셰이더 구조를 이름으로 아이디화 시켜서 같은거면 그냥 들고 있는걸 쓰는거*/
	
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

HRESULT CRenderSystem::Get_BufferInputLayout(class CVIBuffer* pBuffer, CShader* pShader, const string& passConstant, ID3D11InputLayout** ppInputLayout)
{
	if ( !pBuffer||!pShader || !ppInputLayout)
		return E_FAIL;

	string LayOutID = "Engine_" + pBuffer->Get_Key() +"_" + pShader->Get_Key();

	auto iter = m_InputLayouts.find(LayOutID);

	if (iter != m_InputLayouts.end()) {
		*ppInputLayout = iter->second;
		return S_OK;
	}

	D3DX11_PASS_DESC passDesc = {};

	if (FAILED(pShader->GetPassSignature(passConstant, &passDesc)))
		return E_FAIL;

	if (pBuffer->Get_ElementCount() == 0 || pBuffer->Get_ElementDesc() == nullptr)
		return E_FAIL;

	HRESULT hr = m_pDevice->CreateInputLayout(
		pBuffer->Get_ElementDesc(), pBuffer->Get_ElementCount(),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		ppInputLayout);

	if (FAILED(hr))
		return E_FAIL;

	m_InputLayouts.emplace(LayOutID, *ppInputLayout);

	return S_OK;
}

HRESULT CRenderSystem::Add_Palette(const string& ConstantName, CTexture* pTexture)
{
	if (m_pPipeLine)
		return m_pPipeLine->Add_Palette(ConstantName,pTexture);
	else
		return E_FAIL;
}

void CRenderSystem::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pPipeLine);

	Safe_Release(m_pOpaquePass);
	Safe_Release(m_pInstancePass);
	Safe_Release(m_pUIPass);
	Safe_Release(m_pDebugPass);
	Safe_Release(m_pTargetManager);
	
	for (auto& pair : m_InputLayouts)
		Safe_Release(pair.second);

	m_InputLayouts.clear();
}
