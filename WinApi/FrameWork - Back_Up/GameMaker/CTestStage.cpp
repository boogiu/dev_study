#include "pch.h"
#include "CTestStage.h"
#include "CResourceMgr.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CCamera.h"

CTestStage::CTestStage()
{
}

CTestStage::~CTestStage()
{
	Release();
}

void CTestStage::Initialize()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Platform.bmp", L"Platform");
	__super::Load_Map(L"../TileData/Tile.dat");
	CCamera::Get_Instance()->Set_Target(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
}

void CTestStage::Update()
{
	for (CObject* tile : m_TileList) {
		tile->Update();
	}
}

void CTestStage::Late_Update()
{
}

void CTestStage::Render(HDC _hDC)
{

	for (CObject* tile : m_TileList) {
		tile->Render(_hDC);
	}
}

void CTestStage::Release()
{

}
