#include "pch.h"
#include "CAttackBox.h"
#include "CCamera.h"
#include "CCollisionMgr.h"
#include "CRader.h"
#include "CObject.h"
CAttackBox::CAttackBox(CObject* owner)
    :m_atkTarget(), m_bActive(false), m_tInfo{},m_tRect{}, m_pOwner(owner),m_eCollTyp(ONE_TIME), maxCount(3)
{
}

CAttackBox::~CAttackBox()
{
	m_HittedContainer.clear();
}

void CAttackBox::Render(HDC _hDC)
{
	INFO_EX worldInfo = CCamera::Get_Instance()->WorldToScreen(m_tInfo);
	short i = 0;
	//if (m_pOwner) {
	//	if (dynamic_cast<CRader*>(m_pOwner))
	//	{
	//		i = 255;
	//	}
	//}

	// 초록색 펜 (테두리용)
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, i));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	// 내부를 비우기 위해 NULL_BRUSH 사용
	
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));
	Rectangle(_hDC,
		(int)(worldInfo.fX - worldInfo.offsetLeft),
		(int)(worldInfo.fY - worldInfo.offsetTop),
		(int)(worldInfo.fX + worldInfo.offsetRight),
		(int)(worldInfo.fY + worldInfo.offsetBottom)
	);
	// 원래 브러시와 펜으로 복원 및 자원 해제
	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}


void CAttackBox::Update()
{
	m_tInfo.Update_INFO(m_tRect);

}

void CAttackBox::Late_Update()
{
	
}

bool CAttackBox::isActive()
{
    return m_bActive;
}

void CAttackBox::Set_Active()
{
	m_bActive = true;
	CCollisionMgr::Get_Instance()->Register_AtkBox(this);
}

void CAttackBox::Set_DeActive()
{
	m_bActive = false;
	m_HittedContainer.clear();
}

bool CAttackBox::Can_Hit(CObject* obj)
{
	if (m_HittedContainer.empty()) return true;
	if (m_eCollTyp == CONSTANT) {
		return true;
	}
	if(m_HittedContainer.size() > maxCount) {
		return false;
	}
	auto iter = find_if(m_HittedContainer.begin(), m_HittedContainer.end(), [obj](CObject* data)->bool {return obj == data;});
	if (iter != m_HittedContainer.end()) { return false; }

	return true;
}

OBJ_TAG CAttackBox::Get_Target()
{
	return m_atkTarget;
}

void CAttackBox::Set_Target(OBJ_TAG tag)
{
	m_atkTarget = tag;
}

COLLIDER_INFO& CAttackBox::Get_Info()
{
	return m_tInfo;
}

void CAttackBox::OnCollisionEnter(CObject* target, RECT rc, HIT_TYPE _type)
{
	if (m_eCollTyp == ONE_TIME) {
		m_HittedContainer.push_back(target);
	}
	m_pOwner->OnCollisionEnter(target, rc, _type);
}


CObject* CAttackBox::Get_Owner()
{
	return m_pOwner;
}

void CAttackBox::Clear_Hitted()
{
	m_HittedContainer.clear();
}

void CAttackBox::Update_Rect()
{
	m_tRect.left = m_tInfo.fX - m_tInfo.offsetLeft;
	m_tRect.right = m_tInfo.fX + m_tInfo.offsetRight;
	m_tRect.top = m_tInfo.fY - m_tInfo.offsetTop;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.offsetBottom;
}
