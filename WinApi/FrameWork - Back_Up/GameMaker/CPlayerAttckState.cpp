#include "pch.h"
#include "CPlayerAttckState.h"
#include "CPlayer.h"
#include "CKeyMgr.h"

CPlayerAttckState::CPlayerAttckState()
{
}

CPlayerAttckState::~CPlayerAttckState()
{
}

void CPlayerAttckState::Enter(CPlayer* player)
{
	if (player->GetState() == CPlayer::ATTACKB) {
		player->SetAnim(L"ATTACKB");
	}
	else if(player->GetState() == CPlayer::ATTACKA) {
		player->SetAnim(L"ATTACKA");
	}
}

void CPlayerAttckState::Update(CPlayer* player)
{
	if (player->isAnimEnd()) {
		if (CKeyMgr::Get_Instance()->GetKeyDown('X')) {
			if (player->GetState() == CPlayer::ATTACKB) {
				player->ChangeState(CPlayer::ATTACKA);
				return;
			}
			else if (player->GetState() == CPlayer::ATTACKA) {
				player->ChangeState(CPlayer::ATTACKB);
				return;
			}
		}
		player->ChangeState(CPlayer::IDLE);
	}
}

bool CPlayerAttckState::Exit(CPlayer* player)
{
	return false;
}
