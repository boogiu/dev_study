#include "pch.h"
#include "CRewardBox.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CAnim.h"
#include "CObjMgr.h"
#include "CSkulItem.h"

CRewardBox::~CRewardBox()
{
	Release();
}

CRewardBox::CRewardBox()
	:m_bOpen(false), m_pAnim(nullptr), m_tAnim{}, m_bActivate(false), m_bisOpened(false)
{
	memDC = CResourceMgr::Get_Instance()->Find_Image(L"Stage_Door");
	m_pAnim = new CAnim(this);

	m_tAnim.startPoint = { 0,0 };
	m_tAnim.endPoint = { 1,3 };
	m_tAnim.m_spSize = { 154,150 };
	m_tAnim.LoopPoint = {0,0};
	m_tAnim.isLoop = false;
	m_tAnim.frameSpeed = 0.1f;

	m_pAnim->Set_Anim(L"Skul_Box", m_tAnim);
	m_pAnim->Set_BreakPoint({ 0,0 });
}

CRewardBox::CRewardBox(TILE_INFO _info)
	:m_bOpen(false), m_pAnim(nullptr), m_tAnim{}, m_bActivate(false), m_bisOpened(false)
{

}

void CRewardBox::Initialize()
{
	m_bOpen = false;
}

void CRewardBox::Update()
{
	if (m_bActivate && !m_bisOpened) {
		m_pAnim->Set_Stop(false);
		m_pAnim->Set_BreakPoint({ 1,0 });
		m_bisOpened = true;
	}

	if (CCollisionMgr::Get_Instance()->Trigger_With_Player(this)&& m_bisOpened) {
		if (CKeyMgr::Get_Instance()->GetKeyTap('F')) {
			if (!m_bOpen) {
				CObject* skul = CObjMgr::Get_Instance()->ReQuestObject<CSkulItem>(ITEM);
				static_cast<CSkulItem*>(skul)->Set_Name(L"Wolf_Skul");
				skul->Initialize();
				skul->Set_Pos(m_tInfo.fX, m_tInfo.fY);
				//static_cast<CSkulItem*>(skul)->Set_Origin(m_tInfo.fY);
				m_bOpen = true;
			}
			m_pAnim->Set_Stop(false);
		}
	}
	m_pAnim->Update();
}

void CRewardBox::Late_Update()
{
	
}

void CRewardBox::Render(HDC _hDC)
{
	m_pAnim->Render(_hDC);
}

void CRewardBox::Release()
{
	Safe_Delete<CAnim*>(m_pAnim);
}

void CRewardBox::Set_Tile(TILE_INFO _info)
{
	m_tTile.eType = _info.eType;
	m_tTile.tSprite.m_Col = _info.tSprite.m_Col;
	m_tTile.tSprite.m_Row = _info.tSprite.m_Row;
	m_tTile.tInfo.Copy(_info.tInfo);
	m_tInfo.Copy(_info.tInfo);
	m_pCollider = nullptr;
	m_eMapType = REWARD_BOX;
	INFO_EX BaseInfo = _info.tInfo;
	m_tInfo.Set_Size(300, 250);
	m_tInfo.fY = BaseInfo.RB().y - 115;
}
