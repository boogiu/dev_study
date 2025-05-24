#include "pch.h"
#include "Key_Item.h"
#include "CResourceMgr.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CEntSkul.h"
#include "CEffectMgr.h"

Key_Item::Key_Item()
{
}

Key_Item::~Key_Item()
{
}

void Key_Item::Initialize()
{
	m_tInfo.Translate_Info({ 0, 0, 25,25 });
	elapsedTime = 0;
	m_fFloatAmplitude = 5.0f;
	m_fFloatSpeed = 2.0f;
	m_fOriginY = 0.f; // 일단 0으로, 바닥에 닿으면 저장됨
}

void Key_Item::Update()
{
	float dt = CTimeMgr::Get_Instance()->Get_Delta();
	elapsedTime += dt;
	m_tInfo.fY = m_fOriginY + sinf(elapsedTime * m_fFloatSpeed) * m_fFloatAmplitude;
}

void Key_Item::Late_Update()
{
}

void Key_Item::Render(HDC _hDC)
{
	INFO_EX screenInfo;
	// 중심 기준 좌표 + 크기 정보 포함
	screenInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);

	GdiTransparentBlt(
		_hDC,
		screenInfo.LT().x, screenInfo.LT().y,
		screenInfo.X_Size(), screenInfo.Y_Size(),
		memDC,
		0, 0,
		m_spSize.cx,
		m_spSize.cy,
		HOLLOW
	);
}

void Key_Item::Release()
{
}

void Key_Item::Set_Name(wstring keyName)
{
	m_IConName= keyName;
	memDC = CResourceMgr::Get_Instance()->Find_Image(m_IConName.c_str());
	m_spSize = GetBitmapSizeFromDC(memDC);
}

SIZE Key_Item::GetBitmapSizeFromDC(HDC hMemDC)
{
	SIZE size = { 0, 0 };
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(hMemDC, OBJ_BITMAP);

	if (!hBmp) {
		OutputDebugString(L"[오류] DC에 비트맵이 Select되어 있지 않음\n");
		return size;
	}

	BITMAP bmp{};
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	size.cx = bmp.bmWidth;
	size.cy = bmp.bmHeight;
	return size;
}
