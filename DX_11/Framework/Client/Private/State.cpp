#include "State.h"
#include "Client_Defines.h"

CState::CState()
{
}

void CState::Render_State()
{
}

void CState::OnCollisionEnter(COLLISION_CONTEXT context)
{
}

void CState::OnCollisionStay(COLLISION_CONTEXT context)
{
}

void CState::OnCollisionExit(COLLISION_CONTEXT context)
{
}


void CState::Free()
{
	__super::Free();
}
