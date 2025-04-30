#include "pch.h"
#include "CTestStage.h"
#include "CResourceMgr.h"

CTestStage::CTestStage()
{
}

CTestStage::~CTestStage()
{
}

void CTestStage::Initialize()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Stage_back.bmp", L"Ground");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Platform.bmp", L"Platform");
	__super::Load_Map(L"../TileData/Tile.dat");
}

void CTestStage::Update()
{
}

void CTestStage::Late_Update()
{
}

void CTestStage::Render(HDC _hDC)
{

	HDC hGroundDC = CResourceMgr::Get_Instance()->Find_Image(L"Ground");;
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
	__super::Render_Tile(_hDC);
}

void CTestStage::Release()
{
}
