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
	if (!m_pData) return E_FAIL;

	return S_OK;
}

HRESULT CAnimator3D::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

void CAnimator3D::LinkAnimate_Model(const string& LevelKey, const string& ModelKey)
{
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
	m_fCurrentTrackPosition = nowClip->TranslateAnimateMatrix(m_TransfromationMatrices, m_fCurrentTrackPosition, dt);
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
