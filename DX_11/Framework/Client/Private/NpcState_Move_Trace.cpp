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
    auto& trace = m_pCharacter->Get_TracePack();
    auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();
    m_pCharacter->LookToPlayer(dt);

    if (trace.Player_Near)
        return;

    m_fElapsedTime += dt;

    //1초 간경으로 경로 요청
    if (m_fElapsedTime > m_fRefresh) {
         Request_Path();
    }
    
    TILE_INDEX nextIdx = m_PathIndex[m_nowIndex];
     _float4    nextPos = tileSys->Get_PositionByIndex(nextIdx, ANCHOR::Center);
     _vector curPos = myTransform->Get_Pos();
     _vector target = XMLoadFloat4(&nextPos);
     _vector dir = XMVector3Normalize(target - curPos);
     _float  dist = XMVectorGetX(XMVector3Length(target - curPos));

     //다음 타일에 도착하면 다음 인덱스로
    if (dist < MovePack.fMoveSpeed * dt * 1.1f) {
        ++m_nowIndex;
        if (m_nowIndex >= m_PathIndex.size())
        {
            m_nowIndex -= 1;
            MovePack.vMoveAxis = { 0.f, 0.f };
            return;
        }
    }
    _float4 moveDirection = {};
    XMStoreFloat4(&moveDirection, dir);
    MovePack.vMoveAxis = { moveDirection.x,moveDirection .z};
    XMVectorSetY(dir, MovePack.fCharacterHeight);
    myTransform->Translate(dir * dt* MovePack.fMoveSpeed);
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
    vector<TILE_INDEX> dummy;
    m_PathIndex.swap(dummy);
    m_fElapsedTime = 0.f;
    m_nowIndex = 1;

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
