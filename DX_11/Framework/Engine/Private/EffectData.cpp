#include "EffectData.h"

CEffectData::CEffectData()
{
}

CEffectData* CEffectData::Create()
{
	return new CEffectData;
}

void CEffectData::Free()
{
	__super::Free();
	for (auto Data : m_EmitterDatas)
	{
		if (Data) {
			Safe_Delete(Data);
		}
	}
}
