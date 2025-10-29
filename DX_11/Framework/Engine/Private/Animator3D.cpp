#include "Animator3D.h"
#include "AnimationClip.h"
#include "ModelData.h"
#include "GameInstance.h"
#include "IResourceService.h"

CAnimator3D::CAnimator3D()
{
}

CAnimator3D::CAnimator3D(const CAnimator3D& rhs)
	:CComponent(rhs), m_pAnimClips(rhs.m_pAnimClips), m_pData{ rhs.m_pData },
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

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		m_TransfromationMatrices[i] = m_pData->Get_TransformMatrix(i);
	}
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
	m_pAnimNames.emplace(AnimKey, m_pAnimClips.size() - 1);
	m_pAnimLoops.push_back(Loop);

	return S_OK;
}

void CAnimator3D::Update_Animation(_float dt)
{
	if (m_pAnimClips.empty()) return;

	switch (m_eState)
	{
	case Engine::CAnimator3D::ANIMATOR_STATE::IDLE:
		break;
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

void CAnimator3D::Change_Animation(_uint index, _float convertDuration)
{
	if (index >= m_pAnimClips.size()) return;

	m_eState = ANIMATOR_STATE::CONVERTING;
	m_fPrevTrackPosition = m_fCurrentTrackPosition;
	m_fCurrentTrackPosition = 0;
	m_iNextClipIndex = index;
	m_fConvertDuration = convertDuration;
}

HRESULT CAnimator3D::Change_Animation(string animName, _bool overrideSame, _float convertDuration)
{
	auto iter = m_pAnimNames.find(animName);

	if (iter == m_pAnimNames.end()) return E_FAIL;

	if (iter->second == m_iCurrentClipIndex) {
		if (!overrideSame) {
			m_eState = ANIMATOR_STATE::RUNNING;
			return E_FAIL;
		}
	}


	if (m_iCurrentClipIndex == -1) {
		m_eState = ANIMATOR_STATE::RUNNING;
		m_iCurrentClipIndex = iter->second;
		m_fCurrentTrackPosition = 0.f;
		isAnimEnd = false;
		return S_OK;
	}

	if (m_eState == ANIMATOR_STATE::CONVERTING)
	{
		if (!m_QueuedAnim.IsQueued) {
			m_QueuedAnim.ConvertTime = convertDuration;
			m_QueuedAnim.IsQueued = true;
			m_QueuedAnim.Name = animName;
			m_QueuedAnim.animIndex = iter->second;
		}
		return S_OK;
	}

	m_eState = ANIMATOR_STATE::CONVERTING;
	m_fPrevTrackPosition = m_fCurrentTrackPosition;
	m_fCurrentTrackPosition = 0;
	m_iNextClipIndex = iter->second;
	m_fConvertDuration = convertDuration;

	return S_OK;
}

HRESULT CAnimator3D::ForceChange_Animation(string animName, _bool overrideSame, _float convertDuration)
{
	auto iter = m_pAnimNames.find(animName);

	if (iter == m_pAnimNames.end()) return E_FAIL;

	if (iter->second == m_iCurrentClipIndex) {
		if (!overrideSame) {
			m_eState = ANIMATOR_STATE::RUNNING;
			return E_FAIL;
		}
	}


	if (m_iCurrentClipIndex == -1) {
		m_eState = ANIMATOR_STATE::RUNNING;
		m_iCurrentClipIndex = iter->second;
		m_fCurrentTrackPosition = 0.f;
		isAnimEnd = false;
		return S_OK;
	}

	m_eState = ANIMATOR_STATE::CONVERTING;
	m_fPrevTrackPosition = m_fCurrentTrackPosition;
	m_fCurrentTrackPosition = 0;
	m_iNextClipIndex = iter->second;
	m_fConvertDuration = convertDuration;

	return S_OK;
}

HRESULT CAnimator3D::Stop_Animation()
{
	m_fCurrentTrackPosition = 0;
	m_iCurrentClipIndex = -1;
	return S_OK;
}

HRESULT CAnimator3D::Set_AnimationBlend(string animName, vector<_uint> blendIndex)
{
	m_BlendIndex.clear();

	auto iter = m_pAnimNames.find(animName);

	if (iter == m_pAnimNames.end()) return E_FAIL;

	if (iter->second == m_iCurrentClipIndex) {
		return E_FAIL;
	}

	if (iter->second == m_iBlendAnimation) {
		m_fBlendTrackPosition = 0.f;
		m_eBlendState = BLENDER_STATE::BLEND_IN;
		m_BlendIndex = blendIndex;
		return S_OK;
	}

	m_eBlendState = BLENDER_STATE::BLEND_IN;
	m_fBlendTrackPosition = 0.f;
	m_fBlendDuration = 0;
	m_iBlendAnimation = iter->second;
	m_BlendIndex = blendIndex;
	return S_OK;
}

HRESULT CAnimator3D::Reset_AnimationBlend()
{
	m_eBlendState = BLENDER_STATE::BLEND_OUT;

	return S_OK;
}

_bool CAnimator3D::isCurrentAnimEnd()
{
	if (m_iCurrentClipIndex == -1)
		return true;
	if (m_eState == ANIMATOR_STATE::CONVERTING)
		return false;
	else if (m_pAnimLoops[m_iCurrentClipIndex])
		return false;
	else
		return isAnimEnd;
}

_bool CAnimator3D::isOverAnimTiming(_float percent)
{
	auto& nowClip = m_pAnimClips[m_iCurrentClipIndex];

	return nowClip->Get_Duration() * percent < m_fCurrentTrackPosition;
}

string CAnimator3D::Get_CurrentAnimName()
{
	if (m_eState == ANIMATOR_STATE::CONVERTING)
		return m_pAnimClips[m_iNextClipIndex]->Get_Name();
	else
		return m_pAnimClips[m_iCurrentClipIndex]->Get_Name();
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

void CAnimator3D::Dettach_BoneRelation(_uint Index)
{
	m_DettachedBone.insert(Index);
}

_float4x4 CAnimator3D::Get_BoneMatrix(const string& boneName)
{
	_int Index = m_pData->Find_BoneIndexByName(boneName);
	if (Index == -1)  return _float4x4{};
	else {
		return m_FinalMatices[Index];
	}
}

_float4x4 CAnimator3D::Get_BoneMatrix(_uint Index)
{
	if (Index >= m_ManipulateMatrices.size()) return _float4x4{};
	else {
		return m_FinalMatices[Index];
	}
}

void CAnimator3D::Animation_Run(_float dt)
{
	if (m_iCurrentClipIndex == -1)  return;

	auto& nowClip = m_pAnimClips[m_iCurrentClipIndex];
	m_fCurrentTrackPosition = nowClip->TranslateAnimateMatrix(
		m_TransfromationMatrices, m_fCurrentTrackPosition,
		dt, m_pAnimLoops[m_iCurrentClipIndex], &isAnimEnd);

	if (m_pAnimLoops[m_iCurrentClipIndex] == false && isAnimEnd) {
		m_eState = ANIMATOR_STATE::IDLE;
	}
	Blend_Run(dt);
}

void CAnimator3D::Animation_Convert(_float dt)
{
	m_fCurrentTrackPosition += dt;

	if (m_iCurrentClipIndex == -1) {
		m_fConvertDuration = 0;
		m_fPrevTrackPosition = 0;
		m_fCurrentTrackPosition = 0;
		m_eState = ANIMATOR_STATE::RUNNING;
		m_iCurrentClipIndex = m_iNextClipIndex;
		m_iNextClipIndex = UINT_MAX;
		isAnimEnd = false;
		return;
	}

	_bool ConvertComplete = m_pAnimClips[m_iCurrentClipIndex]->ConvertTo(
		m_TransfromationMatrices,
		*m_pAnimClips[m_iNextClipIndex],
		m_fConvertDuration,
		m_fPrevTrackPosition,
		m_fCurrentTrackPosition);


	if (m_iBlendAnimation != -1) {
		if (!m_BlendTransfomationMatices.empty()) {
			for (size_t i = 0; i < m_BlendIndex.size(); i++)
			{
				_uint idx = m_BlendIndex[i];
				_matrix base = XMLoadFloat4x4(&m_TransfromationMatrices[idx]);
				_matrix blend = XMLoadFloat4x4(&m_BlendTransfomationMatices[idx]);
				_vector baseS, baseR, baseT;
				_vector blendS, blendR, blendT;

				XMMatrixDecompose(&baseS, &baseR, &baseT, base);
				XMMatrixDecompose(&blendS, &blendR, &blendT, blend);

				_vector blendedS = XMVectorLerp(baseS, blendS, m_fBlendDuration);
				_vector blendedT = XMVectorLerp(baseT, blendT, m_fBlendDuration);
				_vector blendedR = XMQuaternionSlerp(baseR, blendR, m_fBlendDuration);

				_matrix BlendedMatrix = XMMatrixAffineTransformation(
					blendedS, XMVectorSet(0.f, 0.f, 0.f, 1.f), blendedR, blendedT);

				XMStoreFloat4x4(&m_TransfromationMatrices[idx], BlendedMatrix);
			}
		}
	}

	if (ConvertComplete) {
		m_fConvertDuration = 0;
		m_fPrevTrackPosition = 0;
		m_fCurrentTrackPosition = 0;
		m_eState = ANIMATOR_STATE::RUNNING;
		m_iCurrentClipIndex = m_iNextClipIndex;
		m_iNextClipIndex = UINT_MAX;
		isAnimEnd = false;
		if (m_QueuedAnim.IsQueued)
		{
			Change_Animation(m_QueuedAnim.Name, m_QueuedAnim.ConvertTime);
			m_QueuedAnim.IsQueued = false;
		}
	}

}

void CAnimator3D::Blend_Run(_float dt)
{

	if (m_iBlendAnimation != -1) {
		auto& blendClip = m_pAnimClips[m_iBlendAnimation];
		_float speed = 1 * dt;

		if (m_eBlendState == BLENDER_STATE::BLEND_PAUSE)
			return;

		if (m_eBlendState == BLENDER_STATE::BLEND_IN) {
			m_fBlendDuration += speed;
		}
		if (m_eBlendState == BLENDER_STATE::BLEND_OUT) {
			m_fBlendDuration -= speed;
		}
		if (m_fBlendDuration > 1.f) {
			m_fBlendDuration = 1.f;
			m_eBlendState = BLENDER_STATE::RUNNING;
		}


		m_BlendTransfomationMatices = m_TransfromationMatrices;

		m_fBlendTrackPosition = blendClip->TranslateAnimateMatrix(
			m_BlendTransfomationMatices, m_fBlendTrackPosition,
			dt, m_pAnimLoops[m_iBlendAnimation], &isBlendAnimEnd);


		for (size_t i = 0; i < m_BlendIndex.size(); ++i)
		{
			_uint idx = m_BlendIndex[i];
			_matrix base = XMLoadFloat4x4(&m_TransfromationMatrices[idx]);
			_matrix blend = XMLoadFloat4x4(&m_BlendTransfomationMatices[idx]);
			_vector baseS, baseR, baseT;
			_vector blendS, blendR, blendT;

			XMMatrixDecompose(&baseS, &baseR, &baseT, base);
			XMMatrixDecompose(&blendS, &blendR, &blendT, blend);

			_vector blendedS = XMVectorLerp(baseS, blendS, m_fBlendDuration);
			_vector blendedT = XMVectorLerp(baseT, blendT, m_fBlendDuration);
			_vector blendedR = XMQuaternionSlerp(baseR, blendR, m_fBlendDuration);

			_matrix BlendedMatrix = XMMatrixAffineTransformation(
				blendedS, XMVectorSet(0.f, 0.f, 0.f, 1.f), blendedR, blendedT);

			XMStoreFloat4x4(&m_TransfromationMatrices[idx], BlendedMatrix);
		}


		if (m_fBlendDuration < 0) {
			m_fBlendDuration = 0;
			m_BlendIndex.clear();
			m_BlendTransfomationMatices.clear();
			m_iBlendAnimation = -1;
			m_eBlendState = BLENDER_STATE::NONE;
		}
	}

}

void CAnimator3D::BuildBone()
{
	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		int parent = m_pData->Get_BoneParentIndex(i);

		if (parent == -1) {
			_matrix MyTransformation =
				XMLoadFloat4x4(&m_ManipulateMatrices[i]) *
				XMLoadFloat4x4(&m_TransfromationMatrices[i]);

			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation);
		}
		else if (m_DettachedBone.count(i)) {
			_matrix MyTransformation =
				XMLoadFloat4x4(&m_ManipulateMatrices[i]) *
				XMLoadFloat4x4(&m_TransfromationMatrices[i]);

			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation);
		}
		else {
			_matrix ParentCombine = XMLoadFloat4x4(&m_CombinedMatrices[parent]);
			_matrix MyTransformation =
				XMLoadFloat4x4(&m_ManipulateMatrices[i])
				* XMLoadFloat4x4(&m_TransfromationMatrices[i]);

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
			Change_Animation(i);
		}
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::PushID(("##" + m_pAnimClips[i]->Get_Name() + "Loop").c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		if (ImGui::Button(string(m_pAnimLoops[i] ? "Do Once" : "Do Loop").c_str(), ImVec2{ childWidth * 0.35f, textLineHeight + 4 }))
			(m_pAnimLoops[i]) = !(m_pAnimLoops[i]);
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
