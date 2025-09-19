#include "Animator3D.h"
#include "AnimationClip.h"
#include "ModelData.h"

CAnimator3D::CAnimator3D()
{
}

CAnimator3D::CAnimator3D(const CAnimator3D& rhs)
	:CComponent(rhs), m_pAnimClips(rhs.m_pAnimClips),m_pData{rhs.m_pData}
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
	_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	m_TransfromationMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_CombinedMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_FinalMatices.resize(m_pData->Get_BoneCount(), IdentityMatrix);

	/*T본 형태로 구성*/
	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
		m_TransfromationMatrices[i] = m_pData->Get_TransformMatrix(i);
	return S_OK;
}

void CAnimator3D::Update_Animation(_float dt)
{
	if (m_pAnimClips.empty()) return;
	/*애니메이터가 가진 매트릭스를 변환할 수 있도록 넘겨줌*/
	m_fCurrentTrackPosition += dt;
	auto& nowClip = m_pAnimClips[m_iCurrentClipIndex];

	m_fCurrentTrackPosition = nowClip->TranslateAnimateMatrix(m_TransfromationMatrices, m_fCurrentTrackPosition);

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
void CAnimator3D::Chane_Animation(_uint index)
{
	if (index >= m_pAnimClips.size()) return;
	m_iCurrentClipIndex = index;
	m_fCurrentTrackPosition = 0;
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
	for (auto& Clip : m_pAnimClips) {
		Safe_Release(Clip);
	}
}
