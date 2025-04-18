#include "pch.h"
#include "CObject.h"

CObject::CObject()
	: m_tRect{ 0,0,0,0 }, m_tInfo{ 0,0,0,0 }, m_tVelocity{ 0,0 }, m_fSpeed(0) ,m_bOut(false), m_bActive(true),m_sState(sIDLE)
{
}

CObject::~CObject()
{
}

void CObject::OnCollisionEnter(CObject* _object)
{

}

void CObject::OnCollisionEnter(CTile* tile)
{
}

void CObject::OnCollisionOut(CObject* _object)
{
}

void CObject::OnCollisionOut(CTile* _tile)
{
}


void CObject::OnHit()
{

}

void CObject::Update_Rect()
{
	m_tRect.left = m_tInfo.fX - (m_tInfo.fCX * 0.5f);
	m_tRect.top = m_tInfo.fY - (m_tInfo.fCY * 0.5f);
	m_tRect.right = m_tInfo.fX + (m_tInfo.fCX * 0.5f);
	m_tRect.bottom = m_tInfo.fY + (m_tInfo.fCY * 0.5f);
}

void CObject::BorderOutCheck()
{

}
