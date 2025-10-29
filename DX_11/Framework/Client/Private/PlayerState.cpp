#include "Client_Defines.h"
#include "PlayerState.h"
#include "Player.h"
CPlayerState::CPlayerState()
{
}


void CPlayerState::Set_Owner(CGameObject* pObject)
{
	m_pPlayer = dynamic_cast<CPlayer*>(pObject);
}

void CPlayerState::Free()
{
	__super::Free();
}
