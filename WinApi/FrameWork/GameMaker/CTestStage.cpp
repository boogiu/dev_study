#include "pch.h"
#include "CTestStage.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCamera.h"
#include "CInfiniteObj.h"

CTestStage::CTestStage()
{
}

CTestStage::~CTestStage()
{
	Release();
}

void CTestStage::Initialize()
{
	CResourceMgr::Get_Instance()->PlayBGM(L"Back_Ground", 0.1f);
	CCamera::Get_Instance()->Set_Border(3000,1500);
	CObject* player = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
	player->Set_MoveSCLimit(false);
	__super::Load_Map(L"../TileData/Tile.dat");

	for (int i = 0; i < 6; ++i)
	{
		int width = 340;
		CInfiniteObj* tmp = new CInfiniteObj;
		tmp->Initialize();
		INFO_EX myInfo;

		myInfo.fX = i * width;
		myInfo.fY = WINCY >> 1;
		myInfo.Add_OffsetX(width * 0.5f, width * 0.5f);
		myInfo.Add_OffsetY((WINCY >> 1) + 50, (WINCY >> 1) + 50);
		tmp->Set_Info(myInfo);
		tmp->Set_SpriteScale(1920, 400);
		tmp->Set_Index(i);
		tmp->LoadSprite(L"BackWall");
		tmp->Set_MoveSpeed(0.3f);

		m_BackStage.push_back(tmp);
	}
	for (int i = 0; i < 5; ++i)
	{
		int width = 80;
		CInfiniteObj* tmp = new CInfiniteObj;
		tmp->Initialize();
		INFO_EX myInfo;
		myInfo.fX = i * width;
		myInfo.fY = -80;
		myInfo.Add_OffsetX(width * 0.5f, width * 0.5f);
		myInfo.Add_OffsetY(0, WINCY / 3);
		tmp->Set_Info(myInfo);
		tmp->Set_SpriteScale(1600, 578);
		tmp->Set_Index(i);
		tmp->LoadSprite(L"BackCutton");
		tmp->Set_MoveSpeed(1.2f);
		m_BackCutton.push_back(tmp);
	}
	for (int i = 0; i < 6; ++i)
	{
		int width = 150;
		CInfiniteObj* tmp = new CInfiniteObj;
		tmp->Initialize();
		INFO_EX myInfo;
		myInfo.fX = i * width;
		myInfo.fY = WINCY -20;
		myInfo.Add_OffsetX(width * 0.5f, width * 0.5f);
		myInfo.Add_OffsetY(WINCY / 2, 0);
		tmp->Set_Info(myInfo);
		tmp->Set_SpriteScale(1920, 383);
		tmp->Set_Index(i);
		tmp->LoadSprite(L"BackBridge");
		tmp->Set_MoveSpeed(0.6f);
		m_BackBridge.push_back(tmp);
	}
}

void CTestStage::Update()
{
	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			
			tile->Update();
		}
	}
	Update_BackObj();
}

void CTestStage::Late_Update()
{
}

void CTestStage::Render(HDC _hDC)
{
	Render_BackObj(_hDC);

	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			if (CCamera::Get_Instance()->Out_of_Camera(tile->Get_Info())) {
				continue;
			}
			tile->Render(_hDC);
		}
	}
}

void CTestStage::Release()
{
	for (CInfiniteObj*& obj : m_BackStage) {
		Safe_Delete<CInfiniteObj*>(obj);
	}
	for (CInfiniteObj*& obj : m_BackCutton) {
		Safe_Delete<CInfiniteObj*>(obj);
	}
	for (CInfiniteObj*& obj : m_BackBridge) {
		Safe_Delete<CInfiniteObj*>(obj);
	}
}
