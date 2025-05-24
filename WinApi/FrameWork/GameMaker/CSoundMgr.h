#pragma once

#include "Define.h"

class CSoundMgr
{
	friend class CResourceMgr;
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	void PlaySound(const wstring& pSoundKey, float fVolume, int* ID, bool Loop);
	void PlaySound(const wstring& pSoundKey, float fVolume, bool Loop);
	void PlayBGM(const wstring& pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopSound(int eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	System* m_pSystem;
	std::unordered_map<std::wstring, FMOD::Sound*> m_SoundMap;
	vector<Channel*> m_ChannelVec;
};

