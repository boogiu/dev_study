#include "Client_Defines.h"
#include "NpcState.h"
#include "NonPlayer.h"

CNpcState::CNpcState()
{
}

void CNpcState::Set_Owner(CGameObject* pObject)
{
	m_pCharacter = dynamic_cast<CNonPlayer*>(pObject);
}

void CNpcState::Free()
{
	__super::Free();
}