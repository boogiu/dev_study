#include "Client_Defines.h"
#include "NpcState_Move_Trace.h"

#include "NonPlayer.h"
#include "Animator3D.h"

#include "GameInstance.h"

CNpcState_Move_Trace::CNpcState_Move_Trace()
{
}

HRESULT CNpcState_Move_Trace::OnEnter()
{
    auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
    Animator->Change_Animation("Move_Walk_F.anim", false);

    auto& MovePack = m_pCharacter->Get_MovementPack();
    auto& PlayerPack = m_pCharacter->Get_TracePack();
    _float2 Axis = { PlayerPack.vLook_Player.x, PlayerPack.vLook_Player.z };
    XMStoreFloat2(&MovePack.vMoveAxis, XMVector2Normalize(XMLoadFloat2(&Axis)));

    return S_OK;
}

void CNpcState_Move_Trace::OnUpdate(_float dt)
{
    auto& MovePack = m_pCharacter->Get_MovementPack();
    auto& PlayerPack = m_pCharacter->Get_TracePack();

    _float2 Axis = { PlayerPack.vLook_Player.x, PlayerPack.vLook_Player.z };
    _float4 nowPos = m_pCharacter->Get_Position();

    _float4 nextPos = { nowPos.x + Axis.x, nowPos.y, nowPos.z + Axis.y, 1.f };

   auto TileSys = CGameInstance::GetInstance()->Get_TileSystem();
  
   TILE_INDEX index = TileSys->Get_IndexByPosition(nextPos);
   TILE_INFO info = TileSys->Get_InfoByIndex(index);

   if ((info.TileFlag & CANT_WALK)!= 0 ) {
       if (nextPos.x != nowPos.x) {
           if()
            MovePack.vMoveAxis = { 0,1 };
        }
        if(nextPos.z != nowPos.z)
            MovePack.vMoveAxis = { 1,0 };
   }
   else {
       MovePack.vMoveAxis =Axis;
   }


   m_pCharacter->Get_Component<CTransform>()->Translate({ 
       MovePack.fMoveSpeed*MovePack.vMoveAxis.x * dt,
       MovePack.fCharacterHeight,
       MovePack.fMoveSpeed* MovePack.vMoveAxis.y* dt });
}

HRESULT CNpcState_Move_Trace::OnExit()
{
    return S_OK;
}

CState* CNpcState_Move_Trace::HandleTransition()
{
    return nullptr;
}

CNpcState_Move_Trace* CNpcState_Move_Trace::Create()
{
    return new CNpcState_Move_Trace;
}
void CNpcState_Move_Trace::Free()
{
}
