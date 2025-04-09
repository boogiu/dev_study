#include "pch.h"
#include "CMainGame.h"

#include "CKeyMgr.h"

#include "CObj.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CMonster.h"

CMainGame::CMainGame()
	:m_hDC(NULL),m_Player(nullptr),m_KeyMgr(nullptr),m_dwTime(GetTickCount()),m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
	ZeroMemory(m_szPos, sizeof(m_szPos));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	
	if (!m_KeyMgr) {
		m_KeyMgr = new CKeyMgr;
		m_KeyMgr->Initialize();
	}
	
	if (!m_Player) {
		m_Player = new CPlayer;
		m_Player->Initialize();
		static_cast<CPlayer*>(m_Player)->SetKeyMgr(m_KeyMgr);
		static_cast<CPlayer*>(m_Player)->SetBulletList(&m_BulletList);
	}

	if (m_MonsterList.empty()) {
		for (int i = 0; i < 5; ++i) {
			CObj* tmp = new CMonster;
			tmp->Initialize();
			tmp->SetPos((WINCX / 5) * (i + 1), 30.f);

			m_MonsterList.push_back(tmp);
		}
	}
}

void CMainGame::Update()
{
	m_Player->Update();
	if (!m_BulletList.empty()) {
		UpdateBulletList();
	}
	if (!m_MonsterList.empty()) {
		UpdateMonsterList();
	}

	if (!(m_MonsterList.empty()) && !(m_BulletList.empty())) {
		UpdateCollision();
	}
}

void CMainGame::Render()
{
	Rectangle(m_hDC, 0, 0, WINCX+10, WINCY+10); // 화면을 덮는 임시 캔버스

	m_Player->Render(m_hDC);
	
	if (!m_BulletList.empty()) {
		for (CObj* Bullet : m_BulletList) {
			Bullet->Render(m_hDC);
		}
	}

	if (!m_MonsterList.empty()) {
		for (CObj* Monster : m_MonsterList) {
			Monster->Render(m_hDC);
		}
	}

	++m_iFPS;
	RenderFPS();
	RenderPos();
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	if (m_Player) {
		Safe_Delete<CObj*>(m_Player);
	}

	if (m_KeyMgr) {
		Safe_Delete<CKeyMgr*>(m_KeyMgr);
	}

	for (CObj* Bullet: m_BulletList) {
		Safe_Delete<CObj*>(Bullet);
	}

	for (CObj* Monster : m_MonsterList) {
		Safe_Delete<CObj*>(Monster);
	}
}

void CMainGame::RenderFPS()
{

	if (m_dwTime + 1000 < GetTickCount()) //프레임 표시
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
}

void CMainGame::UpdateBulletList()
{
	auto iter = m_BulletList.begin();

	for (;iter != m_BulletList.end();) {
		if ((*iter)->GetBorderOut()) {
			Safe_Delete<CObj*>(*iter);
			iter =m_BulletList.erase(iter);
		}
		else {
			(*iter)->Update();
			++iter;
		}
	}
}

void CMainGame::UpdateMonsterList()
{
	auto iter = m_MonsterList.begin();

	for (;iter != m_MonsterList.end();) {

		if (static_cast<CMonster*>(*iter)->IsDead()) {
			Safe_Delete<CObj*>(*iter);
			iter = m_MonsterList.erase(iter);
		}
		else {
			(*iter)->Update();
			++iter;
		}
	}
}

void CMainGame::UpdateCollision()
{
	auto MonsterIter = m_MonsterList.begin();

	for (;MonsterIter != m_MonsterList.end(); ++MonsterIter) {
		
		auto BulletIter = m_BulletList.begin();

		for (;BulletIter != m_BulletList.end(); ) {
			if (CCollision::CollisionCheck(*MonsterIter, *BulletIter)) {
				static_cast<CMonster*>(*MonsterIter)->GetHit();
				
				Safe_Delete<CObj*>(*BulletIter);
				BulletIter = m_BulletList.erase(BulletIter);
			}
			else {
				++BulletIter;
			}
		}

	}
}




void CMainGame::RenderPos()
{
	RECT posRect = m_Player->GetRECT();
	swprintf_s(m_szPos, L"(%.f , %.f)", m_Player->GetPos().fX, m_Player->GetPos().fY);
	DrawText(m_hDC, m_szPos, lstrlen(m_szPos), &posRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

