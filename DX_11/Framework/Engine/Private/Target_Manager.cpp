#include "Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
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
	ID3D11RenderTargetView* pRenderTargets[8] = {};
	UINT iNumRenderTargets = 0;

	// MRT  DepthStencileView
	ID3D11DepthStencilView* pMrtDSV = nullptr;

	for (auto& pRenderTarget : pMRTList)
	{
		pRenderTarget->Clear();

		pRenderTargets[iNumRenderTargets] = pRenderTarget->Get_RTV();
		iNumRenderTargets++;

		if (pMrtDSV == nullptr) {
			pMrtDSV = pRenderTarget->Get_DSV();
			m_pContext->ClearDepthStencilView(pMrtDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		}
	}

	// 렌더 타겟, 뎁스 바인딩
	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, pMrtDSV);

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
	static _bool CustomTargetOpen = {};
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
		if (ImGui::Begin("RenderTargets", &TabOpen))
		{
			for (auto& rtPair : m_EngineRenderTargets)
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
							ImVec2(1280 / 5, 720 / 5));
					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(900, 5), ImGuiCond_Always);
	ImGui::Begin("##Custom Render TargetView", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Custom Render TargetView"))
		CustomTargetOpen = !CustomTargetOpen;
	ImGui::End();

	if (CustomTargetOpen) {
		if (ImGui::Begin("CustomRenderTargets", &CustomTargetOpen))
		{
			for (auto& rtPair : m_CustomTargets)
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
							ImVec2(1280 / 5, 1280 / 5));

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();

							// 원본 렌더타겟 크기 가져오기
							auto desc = *pRT->Get_ViewPort();
							float w = (float)desc.Width;
							float h = (float)desc.Height;

							// 너무 크면 화면 넘치니까 스케일 다운
							float maxPreview = 600.f;
							float scale = 1.f;

							if (w > maxPreview || h > maxPreview)
							{
								scale = maxPreview / max(w, h);
							}

							ImVec2 previewSize(w * scale, h * scale);

							// 확대 이미지 출력
							ImGui::Image((ImTextureID)pSRV, previewSize);

							ImGui::EndTooltip();
						}
					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}
}
#endif // _USING_GUI

HRESULT CTarget_Manager::Create_Target(const RenderTargetDesc& targetDesc, _bool Engine)
{
	if (Engine) {
		auto iter = m_EngineRenderTargets.find(targetDesc.Key);
		if (iter != m_EngineRenderTargets.end())
			return E_FAIL;

		CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, targetDesc);

		if (nullptr == pRenderTarget)
			return E_FAIL;
		m_EngineRenderTargets.emplace(targetDesc.Key, pRenderTarget);
		return S_OK;
	}
	else {
		auto iter = m_CustomTargets.find(targetDesc.Key);
		if (iter != m_CustomTargets.end())
			return E_FAIL;

		CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, targetDesc);

		if (nullptr == pRenderTarget)
			return E_FAIL;
		m_CustomTargets.emplace(targetDesc.Key, pRenderTarget);
		return S_OK;
	}
}

CRenderTarget* CTarget_Manager::Get_CustomTarget(const string& strTargetTag)
{
	auto	iter = m_CustomTargets.find(strTargetTag);

	if (iter == m_CustomTargets.end())
		return nullptr;

	return iter->second;
}

void CTarget_Manager::Push_Target(const string& key)
{
	SavedState state;

	m_pContext->OMGetRenderTargets(
		1,
		&state.pPrevRTV,
		&state.pPrevDSV
	);
	m_pContext->RSGetViewports(&state.NumViewports, &state.PrevViewPort);

	m_SaveStates.push(state);

	CRenderTarget* target = Get_CustomTarget(key);
	ID3D11RenderTargetView* rtv = target->Get_RTV();
	ID3D11DepthStencilView* dsv = target->Get_DSV();

	m_pContext->OMSetRenderTargets(1, &rtv, dsv);
	m_pContext->RSSetViewports(1, target->Get_ViewPort());
}

void CTarget_Manager::Pop_Target()
{
	if (m_SaveStates.empty()) return;

	SavedState state = m_SaveStates.top();
	m_SaveStates.pop();

	m_pContext->OMSetRenderTargets(1, &state.pPrevRTV, state.pPrevDSV);
	m_pContext->RSSetViewports(state.NumViewports, &state.PrevViewPort);
	Safe_Release(state.pPrevRTV);
	Safe_Release(state.pPrevDSV);
}


CRenderTarget* CTarget_Manager::Find_RenderTarget(const string& strTargetTag)
{
	auto	iter = m_EngineRenderTargets.find(strTargetTag);

	if (iter == m_EngineRenderTargets.end())
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

	for (auto& Pair : m_EngineRenderTargets)
		Safe_Release(Pair.second);

	for (auto& Pair : m_CustomTargets)
		Safe_Release(Pair.second);

	m_EngineRenderTargets.clear();
	m_CustomTargets.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
