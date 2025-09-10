#include "AudioDevice.h"
#include "SoundData.h"
#include "Transform.h"

constexpr int m_iChannelSize = { 64 };

CAudioDevice::CAudioDevice()
{
}

HRESULT CAudioDevice::Initialize()
{
	//FMOD 초기화
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(m_iChannelSize, FMOD_INIT_NORMAL, 0);

	for(size_t i =0; i<static_cast<int>(SOUND_GROUP::END);++i ){
		FMOD::ChannelGroup* pGroup;
		m_pSystem->createChannelGroup(to_string(i).c_str(), &pGroup);
		m_Groups.push_back(pGroup);
	}

	return S_OK;
}

void CAudioDevice::Update()
{
	if (m_pSystem)
		m_pSystem->update();

    if (m_pTransform) {
        _float3 pos;
        _float3 look;
        _float3 up;
        XMStoreFloat3(&pos, m_pTransform->Get_Pos());
        XMStoreFloat3(&look, m_pTransform->Dir(STATE::LOOK));
        XMStoreFloat3(&up, m_pTransform->Dir(STATE::UP));

        FMOD_VECTOR listenerPos = { pos.x, pos.y, pos.z };
        FMOD_VECTOR listenerVel = { 0, 0, 0 };
        FMOD_VECTOR listenerForward = { look.x, look.y, look.z };
        FMOD_VECTOR listenerUp = { up.x, up.y, up.z };

        m_pSystem->set3DListenerAttributes(0, &listenerPos, &listenerVel, &listenerForward, &listenerUp);
    }
}

void CAudioDevice::StopAll()
{
}

FMOD::System* CAudioDevice::Get_System()
{
	return m_pSystem;
}

void CAudioDevice::Play(AUDIO_PACKET& packet)
{
    if (packet.ppChannelToUpdate != nullptr && 
        *(packet.ppChannelToUpdate) != nullptr)
    {
        FMOD::Channel* pOldChannel = *(packet.ppChannelToUpdate);
        bool isPlaying = false;

        if (pOldChannel->isPlaying(&isPlaying) == FMOD_OK && isPlaying)
        {
            if (packet.iLoopCount == -1)
            {
                return;
            }
        }
    }

    m_pSystem->playSound(
        packet.pSound->Get_SoundData(),
        m_Groups[static_cast<_uint>(packet.m_eGroup)],
        false, // 패킷의 Paused 정보를 사용
        packet.ppChannelToUpdate
    );

    FMOD::Channel* pNewChannel = *(packet.ppChannelToUpdate);

    if (pNewChannel) 
    {
        if (packet.is3DAttribute)
        {
          FMOD_VECTOR pos = { packet.vPosition.x, packet.vPosition.y, packet.vPosition.z };
          FMOD_VECTOR vel = { 0.f, 0.f, 0.f }; 
          pNewChannel->set3DAttributes(&pos, &vel);
        }

        pNewChannel->setVolume(packet.fVolume);
        pNewChannel->setLoopCount(packet.iLoopCount);
    }
}

void CAudioDevice::Set_Listener(CTransform* pTransform)
{
    Safe_Release(m_pTransform);
    m_pTransform = pTransform;
    Safe_AddRef(m_pTransform);
}

CAudioDevice* CAudioDevice::Create()
{
	CAudioDevice* instance = new CAudioDevice();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}

	return instance;
}

void CAudioDevice::Free()
{
	__super::Free();

	if (m_pSystem) {
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
    Safe_Release(m_pTransform);
}
