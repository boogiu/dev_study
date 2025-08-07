#pragma once
#include "IService.h"
class CTimer;

NS_BEGIN(Engine)

class ENGINE_DLL ITimeService abstract:
    public IService
{
protected:
    virtual ~ITimeService() DEFAULT;

public:
    virtual CTimer* Add_Timer(const string& key) PURE;
    virtual void Update_Timer(const string& key) PURE;
    virtual _float Get_DeltaTime(const string& key, _bool raw = false) PURE;
    virtual void Set_TimeScale(const string& key, _float scale) PURE;
};

NS_END
