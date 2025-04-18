#include "pch.h"
#include "CObjMgr.h"
#include "Objects.h"

CObjMgr::CObjMgr()
	:m_pPlayer(nullptr)
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Initialize()
{
	if (!m_pPlayer) {
		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();
	}
	int iSceneCount = static_cast<int>(SCENE::END);

	for (int i = 0; i < iSceneCount; ++i) {
		SCENE nowScene = static_cast<SCENE>(i);
		m_mapMonster.insert({ nowScene, vector<CObject*>(10,nullptr)});
		m_mapBullet.insert({ nowScene, vector<CObject*>(10,nullptr) });
	}
}

void CObjMgr::Update()
{
	
}

void CObjMgr::Render(HDC _hDC)
{
}

void CObjMgr::Release()
{
	if (m_pPlayer) {
		Safe_Delete<CObject*>(m_pPlayer);
	}
	
	int iSceneCount = static_cast<int>(SCENE::END); 
	for (int i = 0; i < iSceneCount; ++i) { //남은 씬 오브젝트 모두 삭제
		SCENE nowScene = static_cast<SCENE>(i);
		ReleaseScene(nowScene);
	}
}

CObject* CObjMgr::GetMonster(SCENE _scene)
{
	for (CObject*& monster : m_mapMonster[_scene]) {
		if (monster == nullptr) {
				monster = new CMonster;
				monster->Initialize();
				return monster;
		}
		if (!(monster->GetActivate())) {
			return monster;
		}
	}

	CObject* newMonster = new CMonster;
	newMonster->Initialize();
	m_mapMonster[_scene].push_back(newMonster);

	return newMonster;
}

CObject* CObjMgr::GetBullet(SCENE _scene)
{
	for (CObject*& bullet : m_mapBullet[_scene]) {
		if (bullet == nullptr) {
			bullet = new CBullet;
			bullet->Initialize();
			return bullet;
		}
		if (!(bullet->GetActivate())) {
			return bullet;
		}
	}

	CObject* newBullet = new CBullet;
	newBullet->Initialize();
	m_mapBullet[_scene].push_back(newBullet);

	return newBullet;
}

void CObjMgr::ReleaseScene(SCENE _scene)
{
	for (CObject*& monster : m_mapMonster[_scene]) {
		Safe_Delete<CObject*>(monster);
	}
	for (CObject*& bullet : m_mapBullet[_scene]) {
		Safe_Delete<CObject*>(bullet);
	}
}

vector<CObject*>& CObjMgr::GetMonsterList(SCENE _scene)
{
	return m_mapMonster[_scene];
}

vector<CObject*>& CObjMgr::GetBulletList(SCENE _scene)
{
	return m_mapBullet[_scene];
}

