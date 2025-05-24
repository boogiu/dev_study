#include "pch.h"
#include <io.h>
#include "CSoundMgr.h"
#include <cstdint> 
CSoundMgr::CSoundMgr()
	:m_pSystem(nullptr), m_ChannelVec((int)CH_MAX, nullptr)
{
	m_ChannelVec.resize(16); // 최소한 몇 개는 확보돼야
}


CSoundMgr::~CSoundMgr()
{
	Release();
}
void CSoundMgr::Initialize()
{

	FMOD_RESULT result;

	// FMOD 시스템 생성
	result = FMOD::System_Create(&m_pSystem);
	if (result != FMOD_OK)
	{
		MessageBoxA(nullptr, FMOD_ErrorString(result), "FMOD Error (System_Create)", MB_OK);
		return;
	}

	// 시스템 초기화
	result = m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK)
	{
		MessageBoxA(nullptr, FMOD_ErrorString(result), "FMOD Error (init)", MB_OK);
		return;
	}

	LoadSoundFile();
}

void CSoundMgr::Update()
{
	if (m_pSystem)
		m_pSystem->update();
}

void CSoundMgr::Release()
{
	// 사운드 메모리 해제
	for (auto& pair : m_SoundMap)
	{
		if (pair.second)
		{
			pair.second->release();
		}
	}
	m_SoundMap.clear();

	// 시스템 종료
	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}
void CSoundMgr::PlaySound(const wstring& pSoundKey, float fVolume, int* ID, bool Loop)
{
	auto iter = m_SoundMap.find(pSoundKey);
	if (iter == m_SoundMap.end())
		return;

	iter->second->setMode(Loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

	FMOD::Channel* pChannel = nullptr;

	FMOD_RESULT result = m_pSystem->playSound(iter->second, 0, false, &pChannel);
	if (result != FMOD_OK || !pChannel)
		return;

	pChannel->setVolume(fVolume);

	int oldestIndex = -1;
	unsigned int oldestDSPTime = UINT_MAX;

	for (int i = 1; i < m_ChannelVec.size(); ++i)
	{
		FMOD::Channel* channel = m_ChannelVec[i];
		bool isPlaying = false;

		if (!channel)
		{
			// 빈 슬롯이면 즉시 등록
			m_ChannelVec[i] = pChannel;
			if (ID) *ID = i;
			return;
		}

		channel->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			m_ChannelVec[i] = pChannel;
			if (ID) *ID = i;
			return;
		}


		// 가장 오래된 채널을 찾기 위한 변수
		int oldestIndex = -1;
		uint64_t oldestDSPTime = UINT64_MAX; // 또는 -1ULL

		for (int i = 1; i < m_ChannelVec.size(); ++i)
		{
			FMOD::Channel* channel = m_ChannelVec[i];
			bool isPlaying = false;

			if (!channel)
			{
				m_ChannelVec[i] = pChannel;
				if (ID) *ID = i;
				return;
			}

			channel->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				m_ChannelVec[i] = pChannel;
				if (ID) *ID = i;
				return;
			}

			uint64_t dspClock = 0;
			channel->getDSPClock(nullptr, &dspClock);
			if (dspClock < oldestDSPTime)
			{
				oldestDSPTime = dspClock;
				oldestIndex = i;
			}
		}

		if (oldestIndex != -1)
		{
			m_ChannelVec[oldestIndex]->stop();
			m_ChannelVec[oldestIndex] = pChannel;
			if (ID) *ID = oldestIndex;
		}
	}

	// 전부 사용 중이면 가장 오래된 채널을 정지 후 덮어쓰기
	if (oldestIndex != -1)
	{
		m_ChannelVec[oldestIndex]->stop();
		m_ChannelVec[oldestIndex] = pChannel;
		if (ID) *ID = oldestIndex;
	}
}

void CSoundMgr::PlaySound(const wstring& pSoundKey, float fVolume, bool Loop)
{
	auto iter = m_SoundMap.find(pSoundKey);
	if (iter == m_SoundMap.end())
		return;

	iter->second->setMode(Loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

	FMOD::Channel* pChannel = nullptr;

	FMOD_RESULT result = m_pSystem->playSound(iter->second, 0, false, &pChannel);
	if (result != FMOD_OK || !pChannel)
		return;

	// 볼륨 먼저 설정
	pChannel->setVolume(fVolume);

	// 가장 오래된 채널을 찾기 위한 변수
	int oldestIndex = -1;
	unsigned int oldestDSPTime = UINT_MAX;

	for (int i = 1; i < m_ChannelVec.size(); ++i)
	{
		FMOD::Channel* channel = m_ChannelVec[i];

		bool isPlaying = false;
		if (!channel)
		{
			// 빈 슬롯이 있으면 바로 사용
			m_ChannelVec[i] = pChannel;
			return;
		}
		else
		{
			channel->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				m_ChannelVec[i] = pChannel;
				return;
			}


			// 가장 오래된 채널을 찾기 위한 변수
			int oldestIndex = -1;
			uint64_t oldestDSPTime = UINT64_MAX; // 또는 -1ULL

			for (int i = 1; i < m_ChannelVec.size(); ++i)
			{
				FMOD::Channel* channel = m_ChannelVec[i];
				bool isPlaying = false;

				if (!channel)
				{
					m_ChannelVec[i] = pChannel;
					//if (ID) *ID = i;
					return;
				}

				channel->isPlaying(&isPlaying);
				if (!isPlaying)
				{
					m_ChannelVec[i] = pChannel;
					//if (ID) *ID = i;
					return;
				}

				uint64_t dspClock = 0;
				channel->getDSPClock(nullptr, &dspClock);
				if (dspClock < oldestDSPTime)
				{
					oldestDSPTime = dspClock;
					oldestIndex = i;
				}
			}

			if (oldestIndex != -1)
			{
				m_ChannelVec[oldestIndex]->stop();
				m_ChannelVec[oldestIndex] = pChannel;
				//if (ID) *ID = oldestIndex;
			}
		}
	}

	// 모든 채널이 사용 중이면 가장 오래된 것을 정지 후 교체
	if (oldestIndex != -1)
	{
		m_ChannelVec[oldestIndex]->stop();
		m_ChannelVec[oldestIndex] = pChannel;
	}
}

void CSoundMgr::PlayBGM(const wstring& pSoundKey, float fVolume)
{
	// 1. 기존 BGM 재생 중이면 멈추기
	FMOD::Channel* pOldBGM = m_ChannelVec[(int)CHANNELID::CH_BGM];
	if (pOldBGM)
	{
		bool isPlaying = false;
		pOldBGM->isPlaying(&isPlaying);
		if (isPlaying)
		{
			pOldBGM->stop();
		}
		m_ChannelVec[(int)CHANNELID::CH_BGM] = nullptr;
	}

	// 2. 새 BGM 로드
	auto iter = m_SoundMap.find(pSoundKey);
	if (iter == m_SoundMap.end())
		return;

	iter->second->setMode(FMOD_LOOP_NORMAL); // BGM은 루프

	FMOD::Channel* pChannel = nullptr;
	FMOD_RESULT result = m_pSystem->playSound(iter->second, 0, false, &pChannel);

	if (result == FMOD_OK && pChannel)
	{
		pChannel->setVolume(fVolume);
		m_ChannelVec[(int)CHANNELID::CH_BGM] = pChannel;
	}
}


void CSoundMgr::StopSound(CHANNELID eID)
{
	if (m_ChannelVec[(int)eID])
	{
		m_ChannelVec[(int)eID]->stop();
		m_ChannelVec[(int)eID] = nullptr;
	}
}
void CSoundMgr::StopSound(int eID)
{
	if (m_ChannelVec[eID])
	{
		m_ChannelVec[eID]->stop();
		m_ChannelVec[eID] = nullptr;
	}
}
void CSoundMgr::StopAll()
{
	for (int i = 0; i < m_ChannelVec.size(); ++i) {
		bool isPlaying = false;
		if (m_ChannelVec[i]) {
			m_ChannelVec[i]->isPlaying(&isPlaying);
			if (isPlaying)
				OutputDebugStringA(("Channel " + std::to_string(i) + " is playing\n").c_str());
		}
	}

	for (int i = 0; i < m_ChannelVec.size(); ++i)
	{
		if (m_ChannelVec[i])
		{
			if (i == 0) {
				m_ChannelVec[i]->setMode(FMOD_LOOP_OFF);
			}

			m_ChannelVec[i]->stop();
			m_ChannelVec[i] = nullptr;
		}
	}
}
void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (m_ChannelVec[(int)eID])
	{
		m_ChannelVec[(int)eID]->setVolume(fVolume);
	}
}
void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;
	intptr_t handle = _findfirst("../Sound/*.wav", &fd);
	if (handle == -1) return;

	string szCurPath = "../Sound/";
	do
	{
		if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
			continue;

		// 경로 생성
		string szFullPath = szCurPath + fd.name;

		// FMOD 사운드 생성
		FMOD::Sound* pSound = nullptr;
		FMOD_RESULT result = m_pSystem->createSound(szFullPath.c_str(), FMOD_DEFAULT, 0, &pSound);
		if (result != FMOD_OK || !pSound)
		{
			MessageBoxA(nullptr, FMOD_ErrorString(result), "Sound Load Failed", MB_OK);
			continue;
		}

		// 확장자 없는 키 생성
		std::string rawName = fd.name;
		size_t dotPos = rawName.find_last_of('.');
		if (dotPos != std::string::npos)
			rawName = rawName.substr(0, dotPos);

		std::wstring wKey(rawName.begin(), rawName.end());

		// 등록
		m_SoundMap.emplace(wKey, pSound);

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
}
