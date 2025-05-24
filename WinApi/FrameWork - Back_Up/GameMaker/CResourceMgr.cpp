#include "pch.h"
#include "CResourceMgr.h"
#include "CBmpMgr.h"

CResourceMgr* CResourceMgr::m_pInstance = nullptr;

CResourceMgr::CResourceMgr()
	: m_BmpMgr(nullptr)
{
}

CResourceMgr::~CResourceMgr()
{
	Release();
}

void CResourceMgr::Initialize()
{
	if (!m_BmpMgr) {
		m_BmpMgr = new CBmpMgr;
		m_BmpMgr->Initialize();
	}
}

void CResourceMgr::Update()
{
}

void CResourceMgr::Late_Update()
{
}

void CResourceMgr::Render(HDC _hDC)
{
}

void CResourceMgr::Release()
{
	if (m_BmpMgr) {
		delete m_BmpMgr;
		m_BmpMgr = nullptr;
	}
}

HDC CResourceMgr::Find_Image(const TCHAR* pImageKey)
{
	if (m_BmpMgr) {
		return m_BmpMgr->Find_Image(pImageKey);
	}
	return nullptr;
}

void CResourceMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey)
{
	if (m_BmpMgr) {
		m_BmpMgr->Insert_Bmp(pFilePath, pImageKey);
	}
}
