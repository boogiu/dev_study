#pragma once
#include "ISoundService.h"

class CSoundMgr final:
	public ISoundService
{
private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;
	HRESULT Initialize();
public:
	virtual void Update();
	virtual void StopAll();
	virtual FMOD::Sound* Get_Sound(const string& key);//사운드 리소스
	virtual FMOD::System* Get_System();//사운드 시스템
	
	virtual void Set_ResourcePath(const string& key);
	virtual FMOD::Channel* Play_GlobalSound(const string& key);
private:
	void LoadSoundFile();

private:
	FMOD::System* m_pSystem = { nullptr};
	unordered_map<string, FMOD::Sound*> m_Sounds;
	vector<FMOD::Channel*> m_Channels;

public:
	static CSoundMgr* Create();
	virtual void Free() override;
};

