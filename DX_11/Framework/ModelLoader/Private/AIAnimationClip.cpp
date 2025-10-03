#include "Loader_Defines.h"
#include "AIAnimationClip.h"
#include "AIChannel.h"

CAIAnimationClip::CAIAnimationClip()
{
}

HRESULT CAIAnimationClip::Initialize(const aiAnimation* pAIAnimation, CModelData* pData)
{
	m_iNumChannels = pAIAnimation->mNumChannels;
	m_fDuration = pAIAnimation->mDuration;
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
	m_ClipName = pAIAnimation->mName.C_Str();
	
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CAIChannel* pChannel = CAIChannel::Create(pAIAnimation->mChannels[i],  pData);
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAIAnimationClip::Render_GUI()
{
	for (auto& channel : m_Channels)
		channel->Render_GUI();
}

HRESULT CAIAnimationClip::Save_AnimationClip(const string& DirectoryPath)
{
	ofstream ofs;
	string FullPath = DirectoryPath + "\\"+m_ClipName + ".anim";
	ofs.open(FullPath.c_str());

	if (!ofs.is_open()) {
		MessageBoxA(0, string(m_ClipName + " save Failed").c_str(), "Animation Save", MB_OK);
		return E_FAIL;
	}

	ANIMATION_CLIP_HEADER infoHeader = {};
	infoHeader.bLoop = m_bLoop;
	strcpy_s(infoHeader.ClipName, m_ClipName.data());
	infoHeader.fDuration = m_fDuration;
	infoHeader.fTickPerSecond = m_fTickPerSecond;
	infoHeader.iNumChannels = m_iNumChannels;
	ofs.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
	for (size_t i = 0; i < m_Channels.size(); i++)
	{
		static_cast<CAIChannel*>(m_Channels[i])->Save_Channel(ofs);
	}

	ofs.close();
}

CAIAnimationClip* CAIAnimationClip::Create(const aiAnimation* pAIAnimation, CModelData* pData)
{
	CAIAnimationClip* instance = new CAIAnimationClip();

	if (FAILED(instance->Initialize(pAIAnimation, pData))) {
		Safe_Release(instance);
	}
	return instance;
}

void CAIAnimationClip::Free()
{
	__super::Free();
}
