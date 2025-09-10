#include "SoundData.h"
#include "FMOD_Inc/fmod.hpp"
CSoundData::CSoundData(const string& key)
	:m_SoundKey{ key }
{
}

HRESULT CSoundData::Initialize( FMOD::System* pSystem, const string& filePath)
{
	FMOD_RESULT fr =pSystem->createSound(filePath.c_str(), FMOD_3D, nullptr, &m_pSound);

	if (fr == FMOD_OK)
		return S_OK;
	else
		return E_FAIL;
}



CSoundData* CSoundData::Create( FMOD::System* pSystem, const string& filePath, const string& key)
{
	CSoundData* instance = new CSoundData(key);
	if (FAILED(instance->Initialize(pSystem, filePath))) {
		Safe_Release(instance);
		MSG_BOX("Sound Resource Failed to Create : CSoundData");
	}
	return instance;
}

void CSoundData::Free()
{
	m_pSound->release();
}
