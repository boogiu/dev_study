#include "SoundMgr.h"
constexpr int m_iChannelSize = { 64 };

CSoundMgr::CSoundMgr()
{
}

HRESULT CSoundMgr::Initialize()
{
	//FMOD ÃÊ±âÈ­
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(m_iChannelSize, FMOD_INIT_NORMAL, 0);

	return S_OK;
}

void CSoundMgr::Update()
{
	if (m_pSystem)
		m_pSystem->update();
}

void CSoundMgr::StopAll()
{
}

FMOD::Sound* CSoundMgr::Get_Sound(const string& key)
{
	//m_Sounds
	return nullptr;
}

FMOD::System* CSoundMgr::Get_System()
{
	return m_pSystem;
}

void CSoundMgr::Set_ResourcePath(const string& key)
{
	LoadSoundFile();
}

FMOD::Channel* CSoundMgr::Play_GlobalSound(const string& key)
{
	return nullptr;
}


void CSoundMgr::LoadSoundFile()
{
}

CSoundMgr* CSoundMgr::Create()
{
	CSoundMgr* instance = new CSoundMgr();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void CSoundMgr::Free()
{
	for (auto& Pair : m_Sounds)
	{
		if (Pair.second)
			Pair.second->release();
	}

	m_Sounds.clear();

	if (m_pSystem) {
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}
