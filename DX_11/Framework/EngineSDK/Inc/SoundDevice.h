#pragma once
#include "ISoundService.h"

class CSoundDevice final:
	public ISoundService
{
	/*해당 시스템은 사운드 컴포넌트를 구동하는 역할을 한다*/
private:
	CSoundDevice();
	virtual ~CSoundDevice() = default;
	HRESULT Initialize();
public:
	virtual void Update();
	virtual void StopAll();
	virtual FMOD::System* Get_System();//사운드 시스템
	virtual FMOD::Channel* Play_GlobalSound(const string& key);

private:
	FMOD::System* m_pSystem = { nullptr};
	vector<FMOD::Channel*> m_Channels;

public:
	static CSoundDevice* Create();
	virtual void Free() override;
};

