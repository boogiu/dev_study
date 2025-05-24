#include "pch.h"
#include "CAttackLine.h"
#include "CCamera.h"
#include "CCollisionMgr.h"
#include "CObject.h"

CAttackLine::CAttackLine(CObject* owner)
	:m_atkTarget(MAX_OBJECT),m_bActive(false),m_pOwner(owner)
{
}

CAttackLine::~CAttackLine()
{
}
void CAttackLine::Render(HDC _hDC)
{
	LINE_INFO worldInfo = CCamera::Get_Instance()->WorldToScreen(m_tLineInfo);

	// 더 굵고 밝은 붉은색 펜
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 50, 50));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	// 내부 채우지 않음
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));

	// 선 그리기
	MoveToEx(_hDC, worldInfo.LT.x, worldInfo.LT.y, nullptr);
	LineTo(_hDC, worldInfo.RB.x, worldInfo.RB.y);

	// 양 끝점에 작은 원을 그림 (디버깅용)
	Ellipse(_hDC, worldInfo.LT.x - 3, worldInfo.LT.y - 3, worldInfo.LT.x + 3, worldInfo.LT.y + 3);
	Ellipse(_hDC, worldInfo.RB.x - 3, worldInfo.RB.y - 3, worldInfo.RB.x + 3, worldInfo.RB.y + 3);

	// 복원
	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}

void CAttackLine::Update()
{
}

void CAttackLine::Late_Update()
{
}

bool CAttackLine::isActive()
{
	return m_bActive;
}

void CAttackLine::Set_Active()
{
	if (m_bActive) return;

	m_bActive = true;
	CCollisionMgr::Get_Instance()->Register_AtkLine(this);
}

void CAttackLine::Set_DeActive()
{
	m_bActive = false;
	m_HittedContainer.clear();
}

bool CAttackLine::Can_Hit(CObject* obj)
{
	if (m_HittedContainer.empty()) return true;

	auto iter = find_if(m_HittedContainer.begin(), m_HittedContainer.end(), [obj](CObject* data)->bool {return obj == data;});
	if (iter != m_HittedContainer.end()) { return false; }
	return true;
}

OBJ_TAG CAttackLine::Get_Target()
{
	return m_atkTarget;
}

void CAttackLine::Set_Target(OBJ_TAG tag)
{
	m_atkTarget = tag;
}

void CAttackLine::OnCollisionEnter(CObject* target, RECT rc, HIT_TYPE _type)
{
	m_HittedContainer.push_back(target);
	m_pOwner->OnCollisionEnter(target, rc, _type);
}


CObject* CAttackLine::Get_Owner()
{
	return m_pOwner;
}

void CAttackLine::Clear_Hitted()
{
	m_HittedContainer.clear();
}

bool CAttackLine::InterSectLine(RECT rc)
{
	LINE_INFO left = { { rc.left, rc.top }, { rc.left, rc.bottom } };
	LINE_INFO right = { { rc.right, rc.top }, { rc.right, rc.bottom } };
	LINE_INFO top = { { rc.left, rc.top }, { rc.right, rc.top } };
	LINE_INFO bottom = { { rc.left, rc.bottom }, { rc.right, rc.bottom } };

	if (m_tLineInfo.Intersects(left)) return true;
	if (m_tLineInfo.Intersects(right)) return true;
	if (m_tLineInfo.Intersects(top)) return true;
	if (m_tLineInfo.Intersects(bottom)) return true;

	return false;
}
