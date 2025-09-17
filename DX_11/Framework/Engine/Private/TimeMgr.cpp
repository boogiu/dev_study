#include "Engine_Defines.h"
#include "TimeMgr.h"
#include "Timer.h"

CTimeMgr::CTimeMgr()
{
}


CTimer* CTimeMgr::Add_Timer(const string& imguiID)
{
	CTimer* instance =  Find_Timer(imguiID);

	if(instance)
		return instance;

	instance = CTimer::Create();
	m_Timers.insert({ imguiID,instance });

	return instance;
}

void CTimeMgr::Update_Timer(const string& imguiID)
{
	CTimer* instance = Find_Timer(imguiID);

	if (!instance) return;

	instance->Update_Timer();
}

_float CTimeMgr::Get_DeltaTime(const string& imguiID, _bool raw)
{
	CTimer* instance = Find_Timer(imguiID);

	if (!instance) return 0.f;

	return instance->Get_DeltaTime(raw);
}

void CTimeMgr::Set_TimeScale(const string& imguiID, _float scale)
{
	CTimer* instance = Find_Timer(imguiID);

	if (!instance) return ;

	instance->Set_TimeScale(scale);
}

CTimer* CTimeMgr::Find_Timer(const string& imguiID)
{
	auto iter = m_Timers.find(imguiID);

	if (iter != m_Timers.end())
		return iter->second;
	else
		return nullptr;
}

CTimeMgr* CTimeMgr::Create()
{
	CTimeMgr* instance = new CTimeMgr();
	return instance;
}

void CTimeMgr::Free()
{
	__super::Free();
	
	for (auto& Pair : m_Timers) {
		Safe_Release(Pair.second);
	}
	m_Timers.clear();
}
