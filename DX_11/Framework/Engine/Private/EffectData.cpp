#include "EffectData.h"

CEffectData::CEffectData()
{
}

CEffectData* CEffectData::Create(const EffectDataPreset& preset)
{
	CEffectData*  pData = new CEffectData;

	pData->m_bLoop = preset.bLoop;
	pData->m_fDuration = preset.fDuration;
	pData->name = preset.EffectName;

	for (auto& spriteData : preset.SpriteEmitterDatas)
	{
		pData->m_SpriteEmitterDatas.push_back(spriteData);
	}

	return pData;
}

CEffectData* CEffectData::Create()
{
	return new CEffectData;
}

void CEffectData::Free()
{
	__super::Free();
	m_SpriteEmitterDatas.clear();
}
