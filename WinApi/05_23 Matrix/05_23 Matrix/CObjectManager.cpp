#include "pch.h"
#include "CObjectManager.h"
#include "CObject.h"
#include "CTimeManager.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	Release();
}

void CObjectManager::Initialize()
{
}

void CObjectManager::Update()
{
	float dt =CTimeManager::Get_Instance().Get_Delta();
	for (auto& obj : m_ActiveContainer) {
		obj.get()->Update(dt);
	}
}

void CObjectManager::Late_Update()
{
	float dt = CTimeManager::Get_Instance().Get_Delta();
	for (auto& obj : m_ActiveContainer) {
		obj.get()->Late_Update(dt);
	}
}

void CObjectManager::Release()
{
	//컨테이너 데이터 삭제(근데 유니크 포인터라서 ㄱㅊ)
}
