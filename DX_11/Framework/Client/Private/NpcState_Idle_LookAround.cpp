#include "Client_Defines.h"
#include "NpcState_Idle_LookAround.h"

CNpcState_Idle_LookAround::CNpcState_Idle_LookAround()
{
}

HRESULT CNpcState_Idle_LookAround::OnEnter()
{
	return S_OK;
}

void CNpcState_Idle_LookAround::OnUpdate(_float dt)
{
}

HRESULT CNpcState_Idle_LookAround::OnExit()
{
	return S_OK;
}

CState* CNpcState_Idle_LookAround::HandleTransition()
{
	return nullptr;
}

CNpcState_Idle_LookAround* CNpcState_Idle_LookAround::Create()
{
	return new CNpcState_Idle_LookAround;
}
void CNpcState_Idle_LookAround::Free()
{
	__super::Free();
}
