#pragma once
#include "ITimeService.h"
class CTimer;

NS_BEGIN(Engine)

class CTimeMgr final :
    public ITimeService
{
private:
     CTimeMgr();
    virtual ~CTimeMgr()  DEFAULT;

public:
    CTimer* Add_Timer(const string& key);
    void Update_Timer(const string& key);
    _float Get_DeltaTime(const string& key,_bool raw = false);
    void Set_TimeScale(const string& key,_float scale);

private:
    CTimer* Find_Timer(const string& key);

private:
    unordered_map<string, CTimer*> m_Timers;
public:
    static CTimeMgr* Create();
    void Free();
};

NS_END