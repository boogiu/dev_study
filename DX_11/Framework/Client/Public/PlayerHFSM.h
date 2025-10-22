#pragma once
#include "Base.h"

NS_BEGIN(Client)

class CPlayerHFSM :
	public CBase
{
private:
	CPlayerHFSM(class CPlayer* pPlayer);
	virtual ~CPlayerHFSM() DEFAULT;

public:
	HRESULT Initialize();

public:
	void Excute(class CState* rootState);
	void Update(_float dt);

public:
	template<typename T>
	T* Add_State(const string& name);
	CState* Get_State(const string& name);
public:
	void Render_State(class CPlayer* pPlayer);
private:
	unordered_map<string, class CState*> m_States;
	class CState* m_pRoot = { nullptr };
	class CState* m_pCurrent = { nullptr };
	class CPlayer* m_pPlayer = { nullptr };
public:
	static CPlayerHFSM* Create(class CPlayer* pPlayer);
	virtual void Free();
};

template<typename T>
inline T* CPlayerHFSM::Add_State(const string& name)
{
	T* State = T::Create();
	State->SetName(name);
	State->Set_Player(m_pPlayer);
	State->Set_HFSM(this);
	m_States.insert({ name, State });
	return State;
}
NS_END
