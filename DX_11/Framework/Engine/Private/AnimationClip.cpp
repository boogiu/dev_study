#include "AnimationClip.h"
#include "Channel.h"

CAnimationClip::CAnimationClip()
{
}

HRESULT CAnimationClip::Initialize()
{
	return S_OK;
}

_float CAnimationClip::TranslateAnimateMatrix( vector<_float4x4>& transfomationMatrices , _float CurrentTrackPosition)
{
  
    _float RealTrackPosition = CurrentTrackPosition * m_fTickPerSecond;

    if (m_bLoop) {
        if (RealTrackPosition > m_fDuration)
            return 0;
    }

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        m_Channels[i]->TranslateAnimateMatrix(transfomationMatrices , RealTrackPosition , m_bLoop);
    }
    
    return CurrentTrackPosition;
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
