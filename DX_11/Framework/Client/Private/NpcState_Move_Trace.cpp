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
    Request_Path();

    return S_OK;
}
void CNpcState_Move_Trace::OnUpdate(_float dt)
{
    auto* myTransform = m_pCharacter->Get_Component<CTransform>();
    auto& MovePack = m_pCharacter->Get_MovementPack();
    auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();

    m_fElapsedTime += dt;
    if (m_fElapsedTime > m_fRefresh) {
         Request_Path();
    }
    
    if (m_nowIndex >= m_PathIndex.size()-1)
        return;

    const TILE_INDEX nextIdx = m_PathIndex[m_nowIndex + 1];
    const _float4    nextPos = tileSys->Get_PositionByIndex(nextIdx, ANCHOR::Center);
    const _vector curPos = myTransform->Get_Pos();
    const _vector target = XMLoadFloat4(&nextPos);
    const _vector dir = XMVector3Normalize(target - curPos);
    const _float  dist = XMVectorGetX(XMVector3Length(target - curPos));

    if (dist < 0.05f) {
        ++m_nowIndex;
        return;
    }

    myTransform->Translate(dir * dt* MovePack.fMoveSpeed);
}


void CNpcState_Move_Trace::OrdinaryMove(_float dt)
{
  //  auto* myTransform = m_pCharacter->Get_Component<CTransform>();
  //  auto& MovePack = m_pCharacter->Get_MovementPack();
  //  auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();
  //
  //  // 추적 대상 방향으로 단순 직진
  //
  //
  //  MovePack.vMoveAxis = { dirAxis4.x, dirAxis4.z };
  //  myTransform->Translate(dir * dt * MovePack.fMoveSpeed);
}

void CNpcState_Move_Trace::Request_ShortPath(TILE_INDEX start, TILE_INDEX blocked)
{
    auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();
    auto& PlayerPack = m_pCharacter->Get_TracePack();
    auto& TilePack = m_pCharacter->Get_TilePack();

    const TILE_INDEX playerIdx = tileSys->Get_IndexByPosition(PlayerPack.Player_Pos);
    m_PathIndex = tileSys->Request_Path_To(TilePack.NowIndex, playerIdx, static_cast<_uint>(CANT_WALK));

    if (m_PathIndex.empty()) return;

    m_nowIndex = 0;    
    m_fElapsedTime = 0.f;
}

void CNpcState_Move_Trace::Request_Path()
{
    auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();
    auto& PlayerPack = m_pCharacter->Get_TracePack();
    auto& TilePack = m_pCharacter->Get_TilePack();

    const TILE_INDEX playerIdx = tileSys->Get_IndexByPosition(PlayerPack.Player_Pos);
    m_PathIndex = tileSys->Request_Path_To(TilePack.NowIndex, playerIdx, static_cast<_uint>(CANT_WALK));

    if (m_PathIndex.empty()) return;

    m_nowIndex = 1;   
    m_fElapsedTime = 0.f;
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
