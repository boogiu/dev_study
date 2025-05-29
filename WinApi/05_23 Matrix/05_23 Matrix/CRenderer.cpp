#include "pch.h"
#include "CRenderer.h"
#include "CObject.h"
#include "CTransform.h"
CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Initialize()
{
}

void CRenderer::Update(float dt)
{
   
}

void CRenderer::Late_Update(float dt)
{
}

void CRenderer::Render(HDC _hDC)
{
	CTransform* transform = m_pOwner->GetComponent<CTransform>();
	RECTANGLE rect = transform->Get_Rectangle();

	MoveToEx(_hDC, rect.tLT.m_vWorldPos.x, rect.tLT.m_vWorldPos.y,nullptr);
	LineTo(_hDC, rect.tLB.m_vWorldPos.x, rect.tLB.m_vWorldPos.y);
	LineTo(_hDC, rect.tRB.m_vWorldPos.x, rect.tRB.m_vWorldPos.y);
	LineTo(_hDC, rect.tRT.m_vWorldPos.x, rect.tRT.m_vWorldPos.y);
	LineTo(_hDC, rect.tLT.m_vWorldPos.x, rect.tLT.m_vWorldPos.y);
}
