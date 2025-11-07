#include "Client_Defines.h"
#include "HubState.h"

CHubState::CHubState()
{
}

void CHubState::Change_State(const string name)
{
	CState* nextState = Get_State(name);

	if (nextState == nullptr || m_pCurrentState == nextState)
		return;

	if (m_pCurrentState)
		m_pCurrentState->OnExit();
	m_pCurrentState = nextState;
	if (m_pCurrentState)
		m_pCurrentState->OnEnter();
}

CState* CHubState::Get_State(const string state)
{
	auto iter = m_SubStates.find(state);
	if (iter == m_SubStates.end())
		return nullptr;
	else
		return iter->second;
}

void CHubState::Free()
{
	__super::Free();

	for (auto& subState : m_SubStates)
	{
		Safe_Release(subState.second);
	}
	m_SubStates.clear();
}