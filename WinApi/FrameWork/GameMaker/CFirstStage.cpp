#include "pch.h"
#include "CFirstStage.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCamera.h"
#include "CInfiniteObj.h"
#include "CScene.h"
#include "CBattleScene.h"

CFirstStage::CFirstStage():m_bisStart(false), m_bStageEnd(false), readyToNext(false)
{
}

CFirstStage::~CFirstStage()
{
	Release();
}

void CFirstStage::Initialize()
{
	__super::Load_Map(L"../TileData/FirstMap.dat");
	
	CCamera::Get_Instance()->Set_Border(MAP_CX, MAP_CY);
	CObject* player = CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER);
	player->Set_MoveSCLimit(false);

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
		myInfo.fY = 0;
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
		myInfo.fY = WINCY;
		myInfo.Add_OffsetX(width * 0.5f, width * 0.5f);
		myInfo.Add_OffsetY(WINCY / 2, 0);
		tmp->Set_Info(myInfo);
		tmp->Set_SpriteScale(1920, 383);
		tmp->Set_Index(i);
		tmp->LoadSprite(L"BackBridge");
		tmp->Set_MoveSpeed(0.6f);
		m_BackBridge.push_back(tmp);
	}


	for (int i = 0; i < m_SpawnContainer.size(); ++i) {
		m_SpawnContainer[i]->Initialize();
	}
	__super::Set_Player_Pos();
}

void CFirstStage::Update()
{
	if (!m_bisStart) {
		CResourceMgr::Get_Instance()->PlaySoundW(L"GlassCrack", 0.04f);
		m_bisStart = true;
	}
	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			if(tile->Is_Activate())
			tile->Update();
		}
	}
	Update_BackObj();
	
}

void CFirstStage::Late_Update()
{
	for (CObject* spawn : m_SpawnContainer) {
		if (spawn->Is_Activate()) {
			m_bStageEnd = false;
			break;
		}
		m_bStageEnd = true;
	}

	if (m_bStageEnd) {
		static_cast<CRewardBox*>(m_RewardBox)->Active_Box();
	}
	if (static_cast<CRewardBox*>(m_RewardBox)->Box_Opend()) {
		static_cast<CStageDoor*>(m_StageDoor)->Open_Door();
	}
	else {

	}
	if (static_cast<CStageDoor*>(m_StageDoor)->Next_Stage()) {
		static_cast<CBattleScene*>(m_pOwnScene)->Change_Stage();
	}
}

void CFirstStage::Render(HDC _hDC)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Ground");
	BitBlt(_hDC, 0, 0, WINCX, WINCY, memDC, 150, 0,SRCCOPY);

	Render_BackObj(_hDC);

	for (int i = 0; i < (int)TILE_END;++i) {
		for (CObject* tile : m_StageTile[(TILE_TYPE)i]) {
			if (CCamera::Get_Instance()->Out_of_Camera(tile->Get_Info())) {
				continue;
			}
			if (tile->Is_Activate())
			tile->Render(_hDC);
		}
	}
}

void CFirstStage::Release()
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
