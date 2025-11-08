#include "Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Add_RenderTarget(const string& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);
#ifdef _USING_GUI
	width = iWidth;
	height=iHeight;
#endif
	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const string& strMRTTag, const string& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	vector<CRenderTarget*>& pMRTList = Find_MRT(strMRTTag);

	if (find(pMRTList.begin(), pMRTList.end(), pRenderTarget) == pMRTList.end()) {
		pMRTList.push_back(pRenderTarget);
		Safe_AddRef(pRenderTarget);
	}

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const string& strMRTTag)
{
	vector<CRenderTarget*>& pMRTList = Find_MRT(strMRTTag);

	if (pMRTList.empty()) {
		MSG_BOX("There is No Render Target  : CTarget_Manager");
		return E_FAIL;
	}

	if (pMRTList.size() > 8) {
		MSG_BOX("MRT Size Was Over 8  : CTarget_Manager");
		return E_FAIL;
	}

	m_pContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDSV);

	_uint		iNumRenderTargets = {};

	ID3D11RenderTargetView* pRenderTargets[8] = {};

	for (auto& pRenderTarget : pMRTList)
	{
		pRenderTarget->Clear();
  		pRenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	ID3D11RenderTargetView* pRTVs[8] = { m_pBackBufferRTV };

	m_pContext->OMSetRenderTargets(8, pRTVs, m_pDSV);

	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDSV);
	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const string& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	/* strMRTTag로 추가되어있었던 렌더타겟들(list<CRenderTarget*>)을 동시에 장치에 바인딩한다. */
	vector<CRenderTarget*>& pMRTList = Find_MRT(strMRTTag);

	if (pMRTList.empty()) {
		MSG_BOX("There is No Render Target  : CTarget_Manager");
		return E_FAIL;
	}

	if (pMRTList.size() > 8) {
		MSG_BOX("MRT Size Was Over 8  : CTarget_Manager");
		return E_FAIL;
	}
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDSV);

	_uint		iNumRenderTargets = {};

	ID3D11RenderTargetView* pRenderTargets[8] = {};


	for (auto& pRenderTarget : pMRTList)
	{
		pRenderTarget->Clear();
		pRenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	ID3D11DepthStencilView* depthView = (pDSV) ? pDSV : m_pDSV;
	if(pDSV)
		m_pContext->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, depthView);
	
	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DepthStencilView* pDSV)
{
	ID3D11RenderTargetView* pRTV = m_pBackBufferRTV;
	m_pContext->OMSetRenderTargets(1, &pRTV, pDSV ? pDSV : m_pDSV);

	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDSV);
	return S_OK;
}


HRESULT CTarget_Manager::Get_TargetParam(const string& strTargetTag, SHADER_PARAM& param)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	param.iSize = 0;
	param.pData = pRenderTarget->Get_SRV();
	param.typeName = "Texture2D";

	return S_OK;
}

#ifdef _USING_GUI
void CTarget_Manager::Render_GUI()
{
	static _bool TabOpen = {};
	ImGui::SetNextWindowPos(ImVec2(600, 5), ImGuiCond_Always);
	ImGui::Begin("##Render TargetView", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Render TargetView"))
		TabOpen = !TabOpen;
	ImGui::End();

	if (TabOpen) {
		if (ImGui::Begin("RenderTargets" ,&TabOpen))
		{
			for (auto& rtPair : m_RenderTargets)
			{
				const string& tag = rtPair.first;
				CRenderTarget* pRT = rtPair.second;
				if (!pRT) continue;

				if (ImGui::TreeNode(tag.c_str()))
				{
					ID3D11ShaderResourceView* pSRV = pRT->Get_SRV();
					if (pSRV)
					{
						ImGui::Image((ImTextureID)pSRV,
							ImVec2(width/5,height/5));
					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}
}
#endif // _USING_GUI


CRenderTarget* CTarget_Manager::Find_RenderTarget(const string& strTargetTag)
{
	auto	iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

vector<CRenderTarget*>& CTarget_Manager::Find_MRT(const string& strMRTTag)
{
	auto iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end()) {
		auto [newIter, inserted] = m_MRTs.emplace(strMRTTag, vector<CRenderTarget*>());
		return newIter->second;
	}

	return iter->second;
}


CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CTarget_Manager(pDevice, pContext);
}

void CTarget_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
