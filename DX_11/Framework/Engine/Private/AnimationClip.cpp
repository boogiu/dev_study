#include "AnimationClip.h"
#include "Channel.h"

CAnimationClip::CAnimationClip()
{
}

HRESULT CAnimationClip::Initialize()
{
	return S_OK;
}

_float CAnimationClip::TranslateAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition, _float dt)
{

	_float RealTrackPosition = CurrentTrackPosition + dt * m_fTickPerSecond;

	if (m_bLoop) {
		if (RealTrackPosition > m_fDuration)
			return 0;
	}

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		m_Channels[i]->TranslateAnimateMatrix(transfomationMatrices, RealTrackPosition, m_bLoop);
	}

	return RealTrackPosition;
}

_bool CAnimationClip::ConvertTo(vector<_float4x4>& transfomationMatrices, CAnimationClip& DestAnimation, _float fConvertDuration, _float PrevTrackPosition, _float ConversionTrackPosition)
{
	for (auto& SrcChannel : m_Channels) {
		CChannel* nextChannel = DestAnimation.Find_ChannelByBoneName(SrcChannel->Get_Name());
		SrcChannel->ConvertAnimateMatrix(transfomationMatrices, 
			fConvertDuration, 
			PrevTrackPosition, 
			ConversionTrackPosition,
			nextChannel);
	}

	if (fConvertDuration <= ConversionTrackPosition)
		return true;
	else
		return false;
}

CChannel* CAnimationClip::Find_ChannelByBoneName(const string& boneName)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)->bool {
		return boneName == pChannel->Get_Name();
		});

	if (iter != m_Channels.end()) {
		return *iter;
	}
	else {
		return nullptr;
	}
}

void CAnimationClip::Render_GUI()
{

}

CAnimationClip* CAnimationClip::Create()
{
	CAnimationClip* instance = new CAnimationClip();
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CAnimationClip::Free()
{
	__super::Free();

	for (auto& channel : m_Channels)
		Safe_Release(channel);
}
