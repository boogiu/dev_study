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
    CTimer* Add_Timer(const string& imguiID);
    void Update_Timer(const string& imguiID);
    _float Get_DeltaTime(const string& imguiID,_bool raw = false);
    void Set_TimeScale(const string& imguiID,_float scale);

private:
    CTimer* Find_Timer(const string& imguiID);

private:
    unordered_map<string, CTimer*> m_Timers;
public:
    static CTimeMgr* Create();
    void Free();
};

NS_END