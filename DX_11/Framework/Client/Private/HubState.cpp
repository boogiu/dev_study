#include "Client_Defines.h"
#include "HubState.h"

CHubState::CHubState()
{
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