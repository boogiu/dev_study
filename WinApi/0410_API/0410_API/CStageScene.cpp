#include "pch.h"
#include "CStageScene.h"
#include "CSceneMgr.h"
#include "CObjMgr.h"
#include "Objects.h"
#include "BackGrounds.h"

CStageScene::CStageScene()
{
}

CStageScene::~CStageScene()
{
	Release();
}

void CStageScene::Initialize()
{
	m_Player = CObjMgr::GetInstance().GetPlayer();
	for (int i = 0; i < 3; ++i) {
		CObject* tmp = CObjMgr::GetInstance().GetMonster(SCENE::STAGE);
		tmp->Activate();
		static_cast<CMonster*>(tmp)->SetTarget(m_Player);
	}
}

void CStageScene::Update()
{
	m_Player->Update();

	for (CObject* monster : CObjMgr::GetInstance().GetMonsterList(SCENE::STAGE)) {
		if (monster && monster->GetActivate()) {
			monster->Update();
		}
	}

	for (CObject* bullet : CObjMgr::GetInstance().GetBulletList(SCENE::STAGE)) {
		if (bullet && bullet->GetActivate()) {
			bullet->Update();
		}
	}
}

void CStageScene::Render(HDC _hDC)
{
	m_Player->Render(_hDC);

	for (CObject* monster : CObjMgr::GetInstance().GetMonsterList(SCENE::STAGE)) {
		if (monster && monster->GetActivate()) {
			monster->Render(_hDC);
		}
	}
	for (CObject* bullet : CObjMgr::GetInstance().GetBulletList(SCENE::STAGE)) {
		if (bullet && bullet->GetActivate()) {
			bullet->Render(_hDC);
		}
	}


	TCHAR m_buff[32];
	RECT tmpRect{ 20,50,200,350 };
	swprintf_s(m_buff, L"total bullet : %d", CObjMgr::GetInstance().GetBulletList(SCENE::STAGE).size());
	DrawText(_hDC, m_buff, lstrlen(m_buff), &tmpRect, DT_CENTER);
}

void CStageScene::Release()
{
	CObjMgr::GetInstance().ReleaseScene(SCENE::STAGE);
}
