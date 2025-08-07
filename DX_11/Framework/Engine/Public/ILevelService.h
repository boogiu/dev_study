#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ILevelService :
    public IService
{
protected:
    virtual ~ILevelService() DEFAULT;

public:
    virtual HRESULT Request_ChangeLevel(string key) PURE;

    virtual void Update(_float dt)PURE;
    virtual HRESULT Render()PURE;

    virtual void Register_Level(string key, LEVEL_CREATOR creator)PURE;
    virtual void Set_LoadingLevel(const string& LoadingKey)PURE;
    virtual const string& Get_NextLevel() PURE;
    virtual void Notify_LoadComplete() PURE;
};

NS_END