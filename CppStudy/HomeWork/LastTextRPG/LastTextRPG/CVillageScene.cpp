#include "pch.h"
#include "CVillageScene.h"
#include "CGameMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"

CVillageScene::CVillageScene()
	: nowState(STATE::FIRST)
{
}

CVillageScene::~CVillageScene()
{
}

void CVillageScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
	if (!m_pPlayer->GetName().empty()) {
		nowState = STATE::SECOND;
	}
}

void CVillageScene::Update()
{
	if (nowState == STATE::FIRST) {
		m_pPlayer->SetName();
		nowState = STATE::SECOND;
	}
	else if (nowState == STATE::SECOND) {
		int iSelect = CInput::SafeInt(1, 4);
		if (iSelect == 1) {
			CSceneMgr::GetInstance().ChangeScene(SCENE::SHOP);
		}
		else if (iSelect == 2) {
			CSceneMgr::GetInstance().ChangeScene(SCENE::MOTEL);
		}
		else if (iSelect == 3) {
			CSceneMgr::GetInstance().ChangeScene(SCENE::MAZE);
		}
		else if (iSelect == 4) {
			CSceneMgr::GetInstance().ChangeScene(SCENE::END);
		}
	}
}

void CVillageScene::Render()
{
	switch (nowState)
	{
	case CVillageScene::STATE::FIRST:
		wcout << L"누군가 이름을 물어봅니다." << endl;
		break;
	case CVillageScene::STATE::SECOND:
		wcout << m_pPlayer->GetName()<<L"로군!" << endl;
		wcout << L"우리 마을에서 무엇을 하고 싶은가?" << endl;
		wcout << endl;
		wcout << L"우리 마을에서 무엇을 하고 싶은가?" << endl;
		wcout << L"1. 상점을 들리고 싶다." << endl;
		wcout << L"2. 여관에서 하루 자고 싶다." << endl;
		wcout << L"3. 사냥터로 향하고 싶다." << endl;
		wcout << L"4. 게임을 종료하고 싶다." << endl;
		break;
	case CVillageScene::STATE::QUEST:
		break;
	default:
		break;
	}

}

void CVillageScene::Release()
{
}
