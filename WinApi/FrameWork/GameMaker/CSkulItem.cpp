#include "pch.h"
#include "CSkulItem.h"
#include "CResourceMgr.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CEntSkul.h"

CSkulItem::CSkulItem()
	:m_bDroped(false)
{

}

CSkulItem::~CSkulItem()
{
	Release();
}
void CSkulItem::Initialize()
{
	if (!m_pCollider) {
		m_pCollider = new CCollider(this, m_tInfo);
		m_pCollider->Set_OffsetX(15.f, 15.f);
		m_pCollider->Set_OffsetY(15.f, 15.f);
	}

	m_tInfo.Translate_Info({ 0, 0, 45, 45 });
	elapsedTime = 0;

	m_tVector.fY = -40.f; // 위로 튀는 초기 속도
	//m_bGround = false;
	m_bDroped = false;
	m_bGround_Once = false;

	// 부유용 설정
	m_fFloatAmplitude = 10.0f;
	m_fFloatSpeed = 5.0f;
	m_fOriginY = 0.f; // 일단 0으로, 바닥에 닿으면 저장됨
	m_fDropDelay = 0.f;
}


void CSkulItem::Update()
{
	float dt = CTimeMgr::Get_Instance()->Get_Delta();

	if (!m_bGround && !m_bGround_Once)
	{ 
		m_tInfo.fY += m_tVector.fY * dt * 10; 
		m_tVector.fY += GRAVITY;
		elapsedTime = 0.f;
	}
	else if (m_bGround_Once)
	{
		elapsedTime += dt;
		m_tInfo.fY = m_fOriginY + sinf(elapsedTime * m_fFloatSpeed) * m_fFloatAmplitude;
	}

	if (m_pCollider) {
		m_pCollider->Update();
		m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);
	}

	if (m_bGround && !m_bGround_Once) {
		m_bGround_Once = true;
		m_fOriginY = m_tInfo.fY;
		m_bDroped = true;
		return;
	}
	if (m_bDroped) {
		if (CKeyMgr::Get_Instance()->GetKeyTap('F')) {
			if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)) {
				CPlayer* player = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
				player->ChangeSkul(m_SkulName, this);
				m_bActive = false;
			}
		}
	}
}


void CSkulItem::Late_Update()
{

}

void CSkulItem::Render(HDC _hDC)
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

void CSkulItem::Release()
{
	Safe_Delete<CCollider*>(m_pCollider);
}

void CSkulItem::Set_Name(wstring skulName)
{
	m_SkulName = skulName + L"_Item";
	memDC = CResourceMgr::Get_Instance()->Find_Image(m_SkulName.c_str());
	m_spSize = GetBitmapSizeFromDC(memDC);
}

SIZE CSkulItem::GetBitmapSizeFromDC(HDC hMemDC)
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