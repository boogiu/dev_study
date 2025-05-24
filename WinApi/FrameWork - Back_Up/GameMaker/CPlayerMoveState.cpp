#include "pch.h"
#include "CPlayerMoveState.h"
#include "PlayerStates.h"
#include "CKeyMgr.h"
#include "CPlayer.h"
#include "CSkul.h"

CPlayerMoveState::CPlayerMoveState()
{
}

CPlayerMoveState::~CPlayerMoveState()
{
}
void CPlayerMoveState::Enter(CPlayer* player)
{
    player->SetAnim(L"MOVE");
}

void CPlayerMoveState::Update(CPlayer* player)
{
    if (CKeyMgr::Get_Instance()->GetKeyDown(VK_LEFT)) {
        player->Add_Velocity(-1.f, 0.f);
    }
    else if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RIGHT)) {
        player->Add_Velocity(1.f, 0.f);
    }
    else {
        player->ChangeState(CPlayer::IDLE);
        return;
    }

    if (!player->isOnGround()) {
        player->ChangeState(CPlayer::FALL);
        return;
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
        player->ChangeState(CPlayer::ATTACKA);
        return;
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
}

bool CPlayerMoveState::Exit(CPlayer* player)
{
    return true;
}
