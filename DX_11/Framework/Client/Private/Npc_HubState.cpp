#include "Client_Defines.h"
#include "Npc_HubState.h"
#include "NonPlayer.h"
#include "NpcState.h"

CNpc_HubState::CNpc_HubState()
{
}

CNpcState* CNpc_HubState::Add_State(CNpcState* pState, string stateName)
{
	auto iter = m_SubStates.emplace(stateName, pState);
	if (iter.second)
	{
		return pState;
	}

	return nullptr;
}

void CNpc_HubState::Set_Owner(CGameObject* pObject)
{
	m_pCharacter = dynamic_cast<CNonPlayer*>(pObject);

	for (auto& subStates : m_SubStates)
	{
		subStates.second->Set_Owner(m_pCharacter);
	}
}

void CNpc_HubState::Free()
{
	__super::Free();
}
