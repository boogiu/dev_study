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
	m_pPipeLine = CPipeLine::Create(m_pDevice,this);
	m_pTargetManager = CTarget_Manager::Create(m_pDevice,m_pContext);
	
	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_pTargetManager->Add_RenderTarget("Target_Diffuse", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));
	m_pTargetManager->Add_RenderTarget("Target_Normal", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));
	m_pTargetManager->Add_RenderTarget("Target_Depth", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.0f, 0.f, 0.f, 0.f));

	m_pTargetManager->Add_RenderTarget("Target_Shadow", g_iMaxWidth, g_iMaxHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f));
	m_pTargetManager->Add_RenderTarget("Target_Shade", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));
	m_pTargetManager->Add_RenderTarget("Target_Specular", ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f));

	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Normal")))
		return E_FAIL;	
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Depth")))
		return E_FAIL;


	if (FAILED(m_pTargetManager->Add_MRT("MRT_LightAcc", "Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_LightAcc", "Target_Specular")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Shadow", "Target_Shadow")))
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
	m_pShadowPass = ShadowPass::Create(this);
	m_pInstancePass = InstancePass::Create(this);
	m_pUIPass = UIPass::Create(this);

#ifdef _DEBUG
	m_pDebugPass = DebugPass::Create(this);
#endif // _DEBUG

	ReadyShadow();
	return S_OK;
}

HRESULT CRenderSystem::Render()
{
	m_pPipeLine->Update_FrameBuffer(m_pContext);
	m_pPipeLine->Update_Frustum();
	Render_Shadow();

	if (FAILED(m_pTargetManager->Begin_MRT("MRT_Deferred"))) return E_FAIL;
		m_pOpaquePass->Execute(m_pContext);
		m_pInstancePass->Execute(m_pContext);
	if (FAILED(m_pTargetManager->End_MRT()))return E_FAIL;
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

	SHADER_PARAM NormalParam = {};
	m_pTargetManager->Get_TargetParam("Target_Normal", NormalParam);
	m_pShader->Bind_Value("g_NormalTexture", NormalParam);

	SHADER_PARAM DepthParam = {};
	m_pTargetManager->Get_TargetParam("Target_Depth", DepthParam);
	m_pShader->Bind_Value("g_DepthTexture", DepthParam);

	SHADER_PARAM WorldMat = { &m_WorldMatrix , "float4x4",sizeof(_float4x4) };
	m_pShader->Bind_Value("g_WorldMatrix", WorldMat);

	m_pPipeLine->Bind_Light(m_pShader, m_pVIBuffer, m_pContext);

	if (FAILED(m_pTargetManager->End_MRT()))return E_FAIL;

	return S_OK;
}

HRESULT CRenderSystem::Render_Combined()
{
	m_pShader->SetConstantBuffer("FrameBuffer", m_pPipeLine->Get_FrameBuffer());
	m_pShader->SetConstantBuffer("ShadowBuffer", m_pPipeLine->Get_ShadowBuffer());

	ID3D11InputLayout* pLayout;
	Get_BufferInputLayout(m_pVIBuffer, m_pShader, "Combined", &pLayout);
	m_pContext->IASetInputLayout(pLayout);

	SHADER_PARAM DiffuseParam = {};
	m_pTargetManager->Get_TargetParam("Target_Diffuse", DiffuseParam);
	m_pShader->Bind_Value("g_DiffuseTexture", DiffuseParam);

	SHADER_PARAM DepthParam = {};
	m_pTargetManager->Get_TargetParam("Target_Depth", DepthParam);
	m_pShader->Bind_Value("g_DepthTexture", DepthParam);

	SHADER_PARAM ShadeParam = {};
	m_pTargetManager->Get_TargetParam("Target_Shade", ShadeParam);
	m_pShader->Bind_Value("g_ShadeTexture", ShadeParam);

	SHADER_PARAM ShadowParam = {};
	m_pTargetManager->Get_TargetParam("Target_Shadow", ShadowParam);
	m_pShader->Bind_Value("g_ShadowTexture", ShadowParam);

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

HRESULT CRenderSystem::ReadyShadow()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iMaxWidth;
	TextureDesc.Height = g_iMaxHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT /* 정적 */;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDepth)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);
	return S_OK;
}

void CRenderSystem::Render_Shadow()
{
	m_pPipeLine->Update_ShadowBuffer(m_pContext);

	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	if (FAILED(m_pTargetManager->Begin_MRT("MRT_Shadow",m_pShadowDepth))) return;
	
	Change_Viewport(g_iMaxWidth, g_iMaxHeight);

	m_pShadowPass->Execute(m_pContext);
	m_pTargetManager->End_MRT(m_pShadowDepth);

	Change_Viewport(ViewportDesc.Width, ViewportDesc.Height);
}

HRESULT CRenderSystem::Change_Viewport(_uint iWidth, _uint iHeight)
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)iWidth;
	ViewPortDesc.Height = (_float)iHeight;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);
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
	Safe_Release(m_pShadowPass);
	Safe_Release(m_pShadowDepth);
	Safe_Release(m_pTargetManager);
	
	for (auto& pair : m_InputLayouts)
		Safe_Release(pair.second);

	m_InputLayouts.clear();
}
