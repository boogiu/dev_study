#include "Client_Defines.h"
#include "NpcState_Move_Walk.h"
#include "NonPlayer.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "Helper_Func.h"
CNpcState_Move_Walk::CNpcState_Move_Walk()
{
}

HRESULT CNpcState_Move_Walk::OnEnter()
{
    auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
    Animator->Change_Animation("Move_Walk_F.anim", false);

    auto& pack = m_pCharacter->Get_MovementPack();
    _float RandomX = Helper::Get_Random_Float(-2, 2);
    _float RandomZ = Helper::Get_Random_Float(-2, 2);
    pack.vMoveAxis.x += RandomX;
    pack.vMoveAxis.y += RandomZ;
    XMStoreFloat2(&pack.vMoveAxis, XMVector2Normalize(XMLoadFloat2(&pack.vMoveAxis)));
    m_fWalkDuration = 0;

    return S_OK;
}

void CNpcState_Move_Walk::OnUpdate(_float dt)
{
    auto& pack = m_pCharacter->Get_MovementPack();
    auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
    m_pCharacter->LookToPlayer(dt);

   m_fWalkDuration += dt;

   if (m_fWalkDuration > 5) {
       Rand_Direction();
   }

 while (true)
   {
       _float4 nowPos = m_pCharacter->Get_Position();
       nowPos.x += pack.fMoveSpeed * pack.vMoveAxis.x * dt;
       nowPos.z += pack.fMoveSpeed * pack.vMoveAxis.y * dt;
       _uint Flag = tileSys->Get_TileFlagByIndex(tileSys->Get_IndexByPosition(nowPos));

       if ((Flag & CANT_WALK) == 0)
           break;
       else
            Rand_Direction();
   }

   m_pCharacter->Get_Component<CTransform>()->Translate({
     pack.fMoveSpeed * pack.vMoveAxis.x * dt,
     pack.fCharacterHeight,
     pack.fMoveSpeed * pack.vMoveAxis.y * dt ,0 });
}

HRESULT CNpcState_Move_Walk::OnExit()
{
    return S_OK;
}

CState* CNpcState_Move_Walk::HandleTransition()
{
    return nullptr;
}
void CNpcState_Move_Walk::Rand_Direction()
{
    auto& pack = m_pCharacter->Get_MovementPack();
    _float RandomX = Helper::Get_Random_Float(-2, 2);
    _float RandomZ = Helper::Get_Random_Float(-2, 2);

    pack.vMoveAxis.x += RandomX;
    pack.vMoveAxis.y += RandomZ;

    XMStoreFloat2(&pack.vMoveAxis, XMVector2Normalize(XMLoadFloat2(&pack.vMoveAxis)));
    m_fWalkDuration = 0;
}
CNpcState_Move_Walk* CNpcState_Move_Walk::Create()
{
    return new CNpcState_Move_Walk;
}
void CNpcState_Move_Walk::Free()
{
    __super::Free();
}
