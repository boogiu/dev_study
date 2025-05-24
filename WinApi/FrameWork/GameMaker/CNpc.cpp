#include "pch.h"
#include "CNpc.h"
#include "CResourceMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CUImgr.h"
#include "CBase_UI.h"

CNpc::CNpc()
	:m_pDialogue(nullptr)
{
}

CNpc::~CNpc()
{
	Release();
}

void CNpc::Initialize()
{
	m_tInfo.Translate_Info({ 0,0,300,300 });
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"NPC_Orge");
	m_pCollider = new CCollider(this);
	m_pCollider->Get_Info().Copy(m_tInfo);
	m_pCollider->Get_Info().Add_OffsetY(-100.f, -15.f);
	m_pCollider->SetActive(true);
	HDC screenDC = GetDC(nullptr);
	whiteDC = CreateCompatibleDC(screenDC);
	hWhiteBitmap = CreateCompatibleBitmap(screenDC, 300, 300);
	SelectObject(whiteDC, hWhiteBitmap);
	ReleaseDC(nullptr, screenDC);

	COLORREF transparentColor = HOLLOW;
	for (int y = 0; y < 300; ++y) {
		for (int x = 0; x < 300; ++x) {
			COLORREF srcColor = GetPixel(memDC, x, y);
			if (srcColor != transparentColor) {
				SetPixel(whiteDC, x, y, RGB(246, 246, 246)); // 흰색
			}
			else {
				SetPixel(whiteDC, x, y, transparentColor);   // 투명색 유지
			}
		}
	}
	AtkCount = 0;
	m_bFree = false;
	m_bTalking = false;
}

void CNpc::Update()
{
	m_pCollider->Update();
	m_pCollider->Update_Center(m_tInfo.fX, m_tInfo.fY);

	if (m_bFlashWhite) {
		m_fFlashTime += CTimeMgr::Get_Instance()->Get_Delta();
		if (m_fFlashTime > 0.1f) {
			m_bFlashWhite = false;
			m_fFlashTime = 0.f;
		}
	}
	if (AtkCount > 5) {
		Set_Coll_Active(false);
		m_bFree = true;
	}
	if (m_bFree) {
		if (CKeyMgr::Get_Instance()->GetKeyTap('F')) {
			if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)) {
				CPlayer* player = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER));
				SetDialougue();
			}
		}
	}
	if (m_bFree&&m_bTalking&& m_pDialogue->Get_Actvie()) {
		if (CKeyMgr::Get_Instance()->GetKeyTap(VK_RETURN)) {
			if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)) {
				CObject* skul = CObjMgr::Get_Instance()->ReQuestObject<CSkulItem>(ITEM);
				static_cast<CSkulItem*>(skul)->Set_Name(L"Ent_Skul");
				skul->Initialize();
				skul->Set_Pos(m_tInfo.fX, m_tInfo.fY);
				m_pDialogue->Set_Active(FALSE);
			}
		}
	}
}

void CNpc::Late_Update()
{
}

void CNpc::Render(HDC _hDC)
{
	int col = 0;
	int row = 0;

	if (AtkCount < 3) {
		col = 0;
		row = 0;
	}
	else if (AtkCount < 6) {
		col = 1;
		row = 0;
	}
	else {
		col = 0;
		row = 1;
	}

	INFO_EX world = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	TransparentBlt(_hDC,
		world.LT().x, world.LT().y,
		world.X_Size(), world.Y_Size(),
		m_bFlashWhite ? whiteDC : memDC,
		col * 150, row * 150,
		150, 150,
		HOLLOW);
}

void CNpc::Release()
{
	Safe_Delete<CCollider*>(m_pCollider);
	if (whiteDC) {
		DeleteObject(hWhiteBitmap);
		DeleteDC(whiteDC);
	}

}
void CNpc::OnCollisionEnter(CObject* object, RECT collRECT, HIT_TYPE _hitType)
{
	if (AtkCount < 6) {// 3단계 x 3타
		AtkCount += 1;
		m_bFlashWhite = true;
		m_fFlashTime = 0.f;
	}
}

void CNpc::SetDialougue()
{
	if (m_bTalking) return;
	m_pDialogue =CUIMgr::Get_Instance()->Request_UI<CDialogueFrame>();
	m_pDialogue->Initialize();
	m_bTalking = true;
}
