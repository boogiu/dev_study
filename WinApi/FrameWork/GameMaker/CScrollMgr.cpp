#include "pch.h"
#include "CScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	:m_fScrollX(0.f),m_fScrollY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Initialize()
{
}

void CScrollMgr::Update()
{
}

void CScrollMgr::Late_Update()
{
}

void CScrollMgr::Render(HDC _hDC)
{
}

void CScrollMgr::Release()
{
}
