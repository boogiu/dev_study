#include "Client_Defines.h"
#include"PlayerState_ToolBase.h"
#include "Player.h"
#include "Animator3D.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "TileSystem.h"

CPlayerState_ToolBase::CPlayerState_ToolBase()
{
}

void CPlayerState_ToolBase::OnEnter()
{
}

void CPlayerState_ToolBase::OnUpdate(_float dt)
{
	
}

void CPlayerState_ToolBase::OnExit()
{
}

CState* CPlayerState_ToolBase::HandleTransition()
{
	
	return nullptr;
}

void CPlayerState_ToolBase::Render_State()
{
}

CPlayerState_ToolBase* CPlayerState_ToolBase::Create()
{
	return new CPlayerState_ToolBase;
}

void CPlayerState_ToolBase::Free()
{
	__super::Free();
}
