#include "Animator3D.h"
#include "AnimationClip.h"
#include "ModelData.h"
#include "GameInstance.h"
#include "IResourceService.h"

CAnimator3D::CAnimator3D()
{
}

CAnimator3D::CAnimator3D(const CAnimator3D& rhs)
	:CComponent(rhs), m_pAnimClips(rhs.m_pAnimClips),m_pData{rhs.m_pData},
	m_TransfromationMatrices{ rhs.m_TransfromationMatrices },
	m_CombinedMatrices{ rhs.m_CombinedMatrices },
	m_FinalMatices{ rhs.m_FinalMatices }
{
	for (auto& Clip : m_pAnimClips) {
		Safe_AddRef(Clip);
	}
		Safe_AddRef(m_pData);
}

HRESULT CAnimator3D::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimator3D::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

void CAnimator3D::LinkAnimate_Model(const string& LevelKey, const string& ModelKey)
{
	Safe_Release(m_pData);
	m_pData = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_ModelData(LevelKey, ModelKey);
	Safe_AddRef(m_pData);
	_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	m_TransfromationMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_CombinedMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_FinalMatices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_ManipulateMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);

	/*T본 형태로 구성*/
	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		int parent = m_pData->Get_BoneParentIndex(i);

		if (parent == -1) {
			m_CombinedMatrices[i] = m_TransfromationMatrices[i];
		}
		else {
			_matrix ParentCombine = XMLoadFloat4x4(&m_CombinedMatrices[parent]);
			_matrix MyTransformation = XMLoadFloat4x4(&m_TransfromationMatrices[i]);
			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation * ParentCombine);
		}
	}

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		XMStoreFloat4x4(&m_FinalMatices[i], m_pData->Get_OffsetMatrix(i) * XMLoadFloat4x4(&m_CombinedMatrices[i]));
	}
}

HRESULT CAnimator3D::Add_AnimClips(const string& LevelKey, const string& AnimKey, const string& Subject, _bool Loop)
{
	if (m_pAnimNames.count(AnimKey))
		return S_OK;

	CAnimationClip* pClips = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_AnimClip(LevelKey, AnimKey, Subject);

	if (!pClips)
		return E_FAIL;

	m_pAnimClips.push_back(pClips);
	m_pAnimNames.emplace(AnimKey, m_pAnimClips.size()-1);
	m_pAnimLoops.push_back(Loop);
	return S_OK;
}

void CAnimator3D::Update_Animation(_float dt)
{
	if (m_pAnimClips.empty()) return;

	switch (m_eState)
	{
	case Engine::CAnimator3D::ANIMATOR_STATE::RUNNING:
		Animation_Run(dt);
		break;
	case Engine::CAnimator3D::ANIMATOR_STATE::CONVERTING:
		Animation_Convert(dt);
		break;
	default:
		break;
	}

	BuildBone();
}

void CAnimator3D::Chane_Animation(_uint index, _float convertDuration)
{
	if (index >= m_pAnimClips.size()) return;

	m_eState = ANIMATOR_STATE::CONVERTING;
	m_fPrevTrackPosition = m_fCurrentTrackPosition;
	m_fCurrentTrackPosition = 0;
	m_iNextClipIndex = index;
	m_fConvertDuration = convertDuration;
}

HRESULT CAnimator3D::Chane_Animation(string animName, _float convertDuration)
{
	auto iter = m_pAnimNames.find(animName);

	if (iter==m_pAnimNames.end()) return E_FAIL;

	m_eState = ANIMATOR_STATE::CONVERTING;
	m_fPrevTrackPosition = m_fCurrentTrackPosition;
	m_fCurrentTrackPosition = 0;
	m_iNextClipIndex = iter->second;
	m_fConvertDuration = convertDuration;

	return S_OK;
}

void CAnimator3D::Control_Bone(const string& boneName, _fmatrix BoneMatrix)
{
	_int Index = m_pData->Find_BoneIndexByName(boneName);
	if (Index == -1) return;

	else {
		XMStoreFloat4x4(&m_ManipulateMatrices[Index], BoneMatrix);
	}
}

void CAnimator3D::Control_BoneByIndex(_uint Index, _fmatrix BoneMatrix)
{
	if (Index >= m_ManipulateMatrices.size()) return;
	else {
		XMStoreFloat4x4(&m_ManipulateMatrices[Index], BoneMatrix);
	}
}

void CAnimator3D::Animation_Run(_float dt)
{
	auto& nowClip = m_pAnimClips[m_iCurrentClipIndex];
	m_fCurrentTrackPosition = nowClip->TranslateAnimateMatrix(m_TransfromationMatrices, m_fCurrentTrackPosition, dt, m_pAnimLoops[m_iCurrentClipIndex]);
}

void CAnimator3D::Animation_Convert(_float dt)
{
	m_fCurrentTrackPosition += dt;

	_bool ConvertComplete = m_pAnimClips[m_iCurrentClipIndex]->ConvertTo(
		m_TransfromationMatrices, 
		*m_pAnimClips[m_iNextClipIndex],
		m_fConvertDuration, 
		m_fPrevTrackPosition, 
		m_fCurrentTrackPosition);

	if (ConvertComplete) {
		m_fConvertDuration = 0;
		m_fPrevTrackPosition = 0;
		m_fCurrentTrackPosition = 0;
		m_eState = ANIMATOR_STATE::RUNNING;
		m_iCurrentClipIndex = m_iNextClipIndex;
		m_iNextClipIndex = 0;
	}

}

void CAnimator3D::BuildBone()
{
	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		int parent = m_pData->Get_BoneParentIndex(i);

		if (parent == -1) {
			m_CombinedMatrices[i] = m_TransfromationMatrices[i];
		}
		else {
			_matrix ParentCombine = XMLoadFloat4x4(&m_CombinedMatrices[parent]);
			_matrix MyTransformation = XMLoadFloat4x4(&m_ManipulateMatrices[i]) *XMLoadFloat4x4(&m_TransfromationMatrices[i]);
			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation * ParentCombine);
		}
	}
	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		XMStoreFloat4x4(&m_FinalMatices[i], m_pData->Get_OffsetMatrix(i) * XMLoadFloat4x4(&m_CombinedMatrices[i]));
	}
}

void CAnimator3D::Render_GUI()
{
	ImGui::SeparatorText("Animator 3D");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 5) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::BeginChild("##Animator 3DChild", ImVec2{ 0, childHeight }, true);
	for (size_t i = 0; i < m_pAnimClips.size(); i++)
	{
		bool isSelected = (m_iCurrentClipIndex == i);
		ImGui::PushID((int)i);
		
		if (ImGui::Selectable(m_pAnimClips[i]->Get_Name().c_str(), isSelected, 0, ImVec2{ childWidth * 0.50f, textLineHeight }))
		{
			Chane_Animation(i);
		}
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::PushID(("##" + m_pAnimClips[i]->Get_Name() + "Loop").c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		if (ImGui::Button(string(m_pAnimLoops[i] ? "Do Once" : "Do Loop").c_str(), ImVec2{ childWidth * 0.35f, textLineHeight + 4 }))
			(m_pAnimLoops[i])=!(m_pAnimLoops[i]);
		ImGui::PopStyleVar();
		ImGui::PopID();


		if (isSelected) {
			ImGui::SetItemDefaultFocus(); // 선택된 항목에 포커스
		}
	}
	ImGui::EndChild();

}

CAnimator3D* CAnimator3D::Create()
{
	CAnimator3D* instance = new CAnimator3D();
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CAnimator3D::Clone()
{
	return new CAnimator3D(*this);
}

void CAnimator3D::Free()
{
	__super::Free();
	Safe_Release(m_pData);
	for (auto& Clip : m_pAnimClips) {
		Safe_Release(Clip);
	}
}
