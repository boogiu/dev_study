#pragma once
#include "IService.h"
NS_BEGIN(Engine)

class ENGINE_DLL ISoundService abstract:
    public IService
{
protected:
    virtual ~ISoundService() DEFAULT;

public:
	virtual void Update() PURE;

	virtual void StopAll() PURE;
	virtual FMOD::Sound* Get_Sound(const string& key) PURE;
	virtual FMOD::System* Get_System() PURE;
	virtual void Set_ResourcePath(const string& key)PURE;
	virtual FMOD::Channel* Play_GlobalSound(const string& key)PURE;
};

NS_END