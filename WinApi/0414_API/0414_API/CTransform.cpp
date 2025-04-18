#include "pch.h"
#include "CTransform.h"

CTransform::CTransform()
{
}

CTransform::~CTransform()
{
}

void CTransform::Initialize()
{
}

void CTransform::Update()
{
}

void CTransform::Late_Update()
{
}

void CTransform::Render(HDC _hDC)
{
	Update_Rect(_hDC);
}

void CTransform::Release()
{
}

void CTransform::Update_Rect(HDC _hDC)
{
	Rectangle(_hDC,
		m_vPos.fX - (m_vScale.fX * 0.5f),
		m_vPos.fY - (m_vScale.fY * 0.5f),
		m_vPos.fX + (m_vScale.fX * 0.5f),
		m_vPos.fY + (m_vScale.fY * 0.5f)
	);
 }
