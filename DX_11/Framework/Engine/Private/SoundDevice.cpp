#include "SoundDevice.h"

constexpr int m_iChannelSize = { 64 };

CSoundDevice::CSoundDevice()
{
}

HRESULT CSoundDevice::Initialize()
{
	//FMOD ÃÊ±âÈ­
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(m_iChannelSize, FMOD_INIT_NORMAL, 0);

	return S_OK;
}

void CSoundDevice::Update()
{
	if (m_pSystem)
		m_pSystem->update();
}

void CSoundDevice::StopAll()
{
}

FMOD::System* CSoundDevice::Get_System()
{
	return m_pSystem;
}

FMOD::Channel* CSoundDevice::Play_GlobalSound(const string& key)
{
	return nullptr;
}

CSoundDevice* CSoundDevice::Create()
{
	CSoundDevice* instance = new CSoundDevice();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}

	return instance;
}

void CSoundDevice::Free()
{
	__super::Free();

	if (m_pSystem) {
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}
