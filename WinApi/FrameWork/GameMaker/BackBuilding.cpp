#include "pch.h"
#include "BackBuilding.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"


BackBuilding::BackBuilding()
{
	m_fMoveSpeed = 0.4f;
}

BackBuilding::~BackBuilding()
{
}

void BackBuilding::Initialize()
{
}

void BackBuilding::Update()
{
}

void BackBuilding::Late_Update()
{
}

void BackBuilding::Render(HDC _hDC)
{
	auto pCam = CCamera::Get_Instance();
	SIZE camSize = pCam->Get_CamSize();
	POINT camPos = pCam->Get_CamPoint();
	POINT camLT = { camPos.x - camSize.cx / 2 ,camPos.y - camSize.cy / 2 };

	HDC memDC = CResourceMgr::Get_Instance()->Find_Image(L"BackBuilding");
	int bgW = 720;
	int bgH = 300;


	int startX = (camLT.x / bgW) * bgW;
	int startY = (camLT.y / bgH) * bgH;


	for (int x = startX - bgW; x < camLT.x + camSize.cx + bgW; x += bgW)
	{
		int screenX = x - (camLT.x * m_fMoveSpeed);

		//TransparentBlt(
		//	_hDC,
		//	screenX, 0,
		//	bgW, camSize.cx,
		//	memDC,
		//	0, 0,
		//	bgW, bgH,
		//	HOLLOW
		//);
	}
}

void BackBuilding::Release()
{
}

void BackBuilding::LoadSprite(const TCHAR* ImgKey)
{
}
