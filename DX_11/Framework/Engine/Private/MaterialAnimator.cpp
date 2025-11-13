#include "MaterialAnimator.h"
#include "Material.h"
#include "MaterialInstance.h"

CMaterialAnimator::CMaterialAnimator()
{
}

CMaterialAnimator::CMaterialAnimator(const CMaterialAnimator& rhs)
	:CComponent(rhs)
{
}

HRESULT CMaterialAnimator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMaterialAnimator::Initialize(COMPONENT_DESC* pArg)
{
	return S_OK;
}

void CMaterialAnimator::Update_Animation(_float dt)
{
	for (auto& [subset, keyframe] : m_NowAnimations)
	{
		auto& vector = m_Clips[subset];
		_uint index = m_ClipNames[keyframe];
		Update_KeyFrame(subset, vector[index], dt);
	}
}

void CMaterialAnimator::LinkAnimate_Material(CMaterial* pMaterial)
{

	if (m_pMasterMaterial)
		Safe_Release(m_pMasterMaterial);
	m_pMasterMaterial = pMaterial;
	Safe_AddRef(m_pMasterMaterial);
}

HRESULT CMaterialAnimator::RegisterKeyFrame(const string& subsetKey, const string& keyframeKey, const MATERIAL_CLIP& clip)
{
	if (auto instance = m_pMasterMaterial->Find_MaterialByName(subsetKey)) {
		auto iter = m_ClipNames.find(keyframeKey);
		if (iter != m_ClipNames.end())
			return E_FAIL;

		MAT_KEYFRAME keyFrame = { clip ,0.f };
		m_Clips[subsetKey].push_back(keyFrame);
		m_ClipNames.emplace(keyframeKey, m_Clips[subsetKey].size() - 1);
		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CMaterialAnimator::Change_Animation(const string& subsetKey, const string& keyframeKey, _bool OverrideSame)
{
	auto iterSub = m_NowAnimations.find(subsetKey);

	if (m_NowAnimations.find(subsetKey) == m_NowAnimations.end()) {
		m_NowAnimations[subsetKey] = keyframeKey;
		iterSub = m_NowAnimations.find(subsetKey);
	}

	auto iterKey = m_ClipNames.find(keyframeKey);
	if (iterKey == m_ClipNames.end())
		return E_FAIL;


	const string& nowKey = iterSub->second; // 현재 실행 중인 키프레임 이름
	_uint prevIndex = m_ClipNames[nowKey];
	_uint nextIndex = m_ClipNames[keyframeKey];

	_uint PrevAnim = m_ClipNames[iterSub->second]; //요청 전에 돌고 있던 키프레임의 위치
	if (iterSub->second == keyframeKey) {
		if (OverrideSame) {
			m_Clips[subsetKey][PrevAnim].Reset(); //초기화 해줌.
		}
		else
			return S_OK;
	}
	/*서브셋 별로 벡터로 들어가 있음*/
	m_Clips[subsetKey][PrevAnim].Reset(); //초기화 해줌.

	/*이제 현재 돌고 있는 애를 요청된 애로 바꿔줌*/
	iterSub->second = keyframeKey;
	m_Clips[subsetKey][nextIndex].Reset();

	return S_OK;
}

void CMaterialAnimator::Update_KeyFrame(const string& subsetKey, MAT_KEYFRAME& KeyFrame, _float dt)
{
	if (KeyFrame.fCurrentTime > KeyFrame.Cilp.fDuration) {
		if (KeyFrame.Cilp.isLoop) {
			KeyFrame.fCurrentTime = 0;
		}
		else {
			return;
		}
	}
	KeyFrame.fCurrentTime += dt * KeyFrame.Cilp.TickperSecond;

	/*전체 사이즈*/
	_uint FrameCount = KeyFrame.Cilp.AnimationKeyFrame.size();
	/*그중 도달 지점*/
	_uint nowIndex = static_cast<_uint>(KeyFrame.fCurrentTime);
	if (nowIndex >= FrameCount) {
	
		nowIndex = FrameCount-1;
	}

	auto matInstance = m_pMasterMaterial->Find_MaterialByName(subsetKey);

	vector<_uint>& OriginState = matInstance->Get_TextureIndex();

	for (_uint i = 0; i < OriginState.size(); i++)
	{
		OriginState[i] = KeyFrame.Cilp.AnimationKeyFrame[nowIndex];
	}
}

CMaterialAnimator* CMaterialAnimator::Create()
{
	CMaterialAnimator* instance = new CMaterialAnimator;
	if (FAILED(instance->Initialize_Prototype())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CMaterialAnimator::Clone()
{
	CMaterialAnimator* instance = new CMaterialAnimator(*this);
	return instance;
}


void CMaterialAnimator::Free()
{
	__super::Free();
	Safe_Release(m_pMasterMaterial);
}
