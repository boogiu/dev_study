#include "pch.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

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
	for (int i = 0; i < (int)OBJ_TAG::MAX_OBJECT; ++i) {
		m_activeObject.insert({ OBJ_TAG(i), vector<CObject*>()});
		m_poolObject.insert({ OBJ_TAG(i), vector<CObject*>() });
	}
}

void CObjMgr::Update()
{
	for (int i = 0; i < (int)OBJ_TAG::MAX_OBJECT; ++i) {
		for (CObject* obj : m_activeObject[OBJ_TAG(i)]) {
			obj->Update();
		}
	}

}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < (int)OBJ_TAG::MAX_OBJECT; ++i) {
		auto iter = m_activeObject[(OBJ_TAG)i].begin();
		for (; iter != m_activeObject[(OBJ_TAG)i].end(); ) {
			if (!(*iter)->Is_Activate()) {
				m_poolObject[(OBJ_TAG)i].push_back(*iter);
				iter = m_activeObject[(OBJ_TAG)i].erase(iter);
			}
			else {
				(*iter)->Late_Update();
				++iter;
			}
		}
	}
}

void CObjMgr::Render(HDC _hDC)
{
	for (CObject* obj : m_activeObject[OBJ_TAG::MONSTER]) {
		obj->Render(_hDC);
	}
	for (CObject* obj : m_activeObject[OBJ_TAG::PLAYER]) {
		obj->Render(_hDC);
	}
	for (CObject* obj : m_activeObject[OBJ_TAG::PROJECTILE]) {
		obj->Render(_hDC);
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < (int)OBJ_TAG::MAX_OBJECT; ++i) {
		for (auto& obj : m_activeObject[OBJ_TAG(i)]) {
			Safe_Delete<CObject*>(obj);
		}
		for (auto& obj : m_poolObject[OBJ_TAG(i)]) {
			Safe_Delete<CObject*>(obj);
		}
	}
	m_activeObject.clear();
	m_poolObject.clear();
}
