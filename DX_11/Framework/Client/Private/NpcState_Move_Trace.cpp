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

    return S_OK;
}
void CNpcState_Move_Trace::OnUpdate(_float dt)
{

   // auto* myTransform = m_pCharacter->Get_Component<CTransform>();
   // auto& MovePack = m_pCharacter->Get_MovementPack();
   // auto  tileSys = CGameInstance::GetInstance()->Get_TileSystem();
   //
   // const _float4 playerPos = m_pCharacter->Get_TracePack().Player_Pos;
   // const _vector curPos = myTransform->Get_Pos();
   // const _vector target = XMLoadFloat4(&playerPos);
   // const _vector dir = XMVector3Normalize(target - curPos);
   //
   // _float4 dirAxis4{};
   // XMStoreFloat4(&dirAxis4, dir);
   // TILE_INDEX step = AxisToStep(dirAxis4.x, dirAxis4.z);
   //
   // // 전방 막힘 체크 → 막히면 짧은 우회
   // TILE_INDEX nowIdx = tileSys->Get_IndexByPosition(m_pCharacter->Get_Position());
   // TILE_INDEX forwardIdx{ nowIdx.IndexX + step.IndexX, nowIdx.IndexZ + step.IndexZ };
   // _uint flag = tileSys->Get_TileFlagByIndex(forwardIdx);
   //    
   // if ((flag & CANT_WALK) != 0) {
   //     Request_ShortPath(nowIdx, forwardIdx);
   //     return;
   // }
   //
   // m_fElapsedTime += dt;
   // if (m_fElapsedTime > m_fRefresh) {
   //     Request_Path();
   // }
   //
   // // 2) 경로가 없으면 "직진 모드"
   // if (m_PathIndex.empty() || m_nowIndex + 1 >= m_PathIndex.size()) {
   //     OrdinaryMove(dt);               // 여기서 실제로 움직이게 구현
   //     return;                         // 경로 추종 블록 건너뜀
   // }
   //
   // // 3) 경로 추종 모드
   // // 다음 타겟 노드
   // const TILE_INDEX nextIdx = m_PathIndex[m_nowIndex + 1];
   // const _float4    nextPos = tileSys->Get_PositionByIndex(nextIdx, ANCHOR::Center);
   //
   // const _vector curPos = myTransform->Get_Pos();
   // const _vector target = XMLoadFloat4(&nextPos);
   // const _vector dir = XMVector3Normalize(target - curPos);
   // const _float  dist = XMVectorGetX(XMVector3Length(target - curPos));
   //
   // // 3-1) 타겟 도착 → 다음 노드로
   // if (dist < 0.05f) {
   //     ++m_nowIndex;
   //     return;
   // }
   //
   // // 3-2) 전방 충돌 검사 (경로 추종 중에도 로컬 회피)
   // _float4 dirAxis4{};
   // XMStoreFloat4(&dirAxis4, dir);
   // TILE_INDEX step = AxisToStep(dirAxis4.x, dirAxis4.z);
   //
   // TILE_INDEX nowIdx = tileSys->Get_IndexByPosition(m_pCharacter->Get_Position());
   // TILE_INDEX forwardIdx{ nowIdx.IndexX + step.IndexX, nowIdx.IndexZ + step.IndexZ };
   //
   // const _uint flag = tileSys->Get_TileFlagByIndex(forwardIdx);
   // if ((flag & CANT_WALK) != 0) {
   //     Request_ShortPath(nowIdx, forwardIdx); // 짧은 우회 경로 요청
   //     return;
   // }
   //
   // // 3-3) 정상 이동
   // MovePack.vMoveAxis = { dirAxis4.x, dirAxis4.z }; // 참고용 축 갱신
   // myTransform->Translate(dir * dt * MovePack.fMoveSpeed);
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

    m_nowIndex = 0;     // ★ 경로의 0번은 현재 위치
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

    m_nowIndex = 0;     // ★ 경로의 0번은 현재 위치
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
