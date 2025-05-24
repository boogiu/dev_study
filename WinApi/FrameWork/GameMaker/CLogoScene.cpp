#include "pch.h"
#include "CLogoScene.h"
#include "CResourceMgr.h"
#include "CUImgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CLogoScene::CLogoScene()
	:memDC(nullptr)
{
}

CLogoScene::~CLogoScene()
{
}

void CLogoScene::Initialize()
{
	CUIMgr::Get_Instance()->UI_Set(false);
	CObjMgr::Get_Instance()->Awake_Pool(false);
	CResourceMgr::Get_Instance()->PlayBGM(L"MainTitle", 0.4f);
}

void CLogoScene::Update()
{
	if (CKeyMgr::Get_Instance()->GetKeyDown(VK_RETURN)) {
		CSceneMgr::Get_Instance()->Change_Scene(BATTLE);
	}
}

void CLogoScene::Late_Update()
{

}

void CLogoScene::Render(HDC _hDC)
{
	HDC memDC = CResourceMgr::Get_Instance()->Find_Image(L"Title_Logo");

	// 이미지 비율 유지 없이 화면 크기에 맞게 출력
	TransparentBlt(_hDC,
		0, 0, WINCX, WINCY,          // 출력 위치와 크기 (전체 화면)
		memDC,
		0, 0, 800,450, // 원본 이미지 위치와 크기
		SRCCOPY);

	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Title_Logo2");
}

void CLogoScene::Release()
{
}

void CLogoScene::CloseScene()
{
	CResourceMgr::Get_Instance()->StopSound(0);
}
