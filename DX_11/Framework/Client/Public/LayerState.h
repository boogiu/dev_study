#pragma once
#include "Base.h"

NS_BEGIN(Client)

class CLayerState final:
	public CBase
{
protected:
	CLayerState();
	virtual ~CLayerState() DEFAULT;

public:
	virtual void Excute(class CState* rootState);
	virtual void Update(_float dt);
	virtual void Request_ChangeState(const string& NextState);

public:
	virtual void OnCollisionEnter(COLLISION_CONTEXT context);
	virtual void OnCollisionStay(COLLISION_CONTEXT context);
	virtual void OnCollisionExit(COLLISION_CONTEXT context);

public: 
	virtual void Render_State();
public:
	template<typename T>
	T* Add_State(const string& name);
	CState* Get_State(const string& name);
	void Set_Machine(class CStateMachine* machine) { m_pStateMachine = machine; }
	_uint Get_CurrentMask();

private:
	class CStateMachine* m_pStateMachine = { nullptr };
	class CState* m_pCurrent = { nullptr };
	class CState* m_pNextState = { nullptr };
	unordered_map<string, class CState*> m_States;

	_bool m_bPendingState = {};
public:
	static CLayerState* Create();
	virtual void Free();
};

template<typename T>
inline T* CLayerState::Add_State(const string& name)
{
	T* State = T::Create();
	State->SetName(name);
	State->SetLayer(this);
	State->Set_Machine(m_pStateMachine);
	m_States.insert({ name, State });

	return State;
}

NS_END
