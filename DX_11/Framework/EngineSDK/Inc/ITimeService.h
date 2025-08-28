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


///밖에 내보낼 서비스 ->서비스 패턴이라고 하드라고 그래서 서비스라고 함