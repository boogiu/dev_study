#include "pch.h"
#include "CPlayerFallState.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "PlayerStates.h"
#include "CSkul.h"
#include "CTimeMgr.h"

CPlayerFallState::CPlayerFallState()
{
}

CPlayerFallState::~CPlayerFallState()
{
}

void CPlayerFallState::Enter(CPlayer* player)
{
    player->SetAnim(L"FALL");
}

void CPlayerFallState::Update(CPlayer* player)
{
    //player->Add_Velocity(0.f, GRAVITY);
    player->Add_Velocity(0.f, GRAVITY * CTimeMgr::Get_Instance()->Get_Delta());
    // 
    // 공중 이동 허용
    // 좌우 이동
    if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT)) {
        player->Add_Velocity(-1.f, 0.f);
    }
    else if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
        player->Add_Velocity(1.f, 0.f);
    }

    if (CKeyMgr::Get_Instance()->GetKeyDown('C')) {
        if (player->CanJump()) {
            player->ChangeState(CPlayer::JUMP);
            return;
        }
    }
    if (CKeyMgr::Get_Instance()->GetKeyTap('Z')) {
        if (player->CanDash()) {
            player->ChangeState(CPlayer::DASH);
            return;
        }
    }
    if (CKeyMgr::Get_Instance()->GetKeyTap('X')) {
        if (player->CanJumpAtk()) {
            player->ChangeState(CPlayer::JUMPATTACK);
            return;
        }
    }

    if (CKeyMgr::Get_Instance()->GetKeyTap('A')) {
        if (player->Get_Skul()->CanSkillA()) {
            player->ChangeState(CPlayer::SKILLA);
            return;
        }
    }

    if (CKeyMgr::Get_Instance()->GetKeyTap('S')) {
        if (player->Get_Skul()->CanSkillB()) {
            player->ChangeState(CPlayer::SKILLB);
            return;
        }
    }
    // 땅에 닿았는지 확인 (충돌처리 함수 또는 바닥 Y값 비교)
    if (player->isOnGround()) {
        player->Set_Velocity(player->Get_Velocity().fX, 0.f);
        player->Set_Pos(player->Get_Info().fX, player->Get_Info().fY);
        player->RestoreJump();
        player->set_JumpAtk(true);
        player->ChangeState(CPlayer::IDLE);
        return;
    }
}

bool CPlayerFallState::Exit(CPlayer* player)
{
    return false;
}
