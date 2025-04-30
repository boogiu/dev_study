#include "pch.h"
#include "CBmpMgr.h"
#include "CBmp.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Initialize()
{
}

void CBmpMgr::Update()
{
}

void CBmpMgr::Late_Update()
{
}

void CBmpMgr::Render(HDC _hDC)
{
}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), [](auto& Pair) {
		if (Pair.second) {
			delete Pair.second;
			Pair.second = nullptr;
		}
		});

	m_mapBit.clear();
}

HDC CBmpMgr::Find_Image(const TCHAR* pImageKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(),
		[pImageKey](auto& Pair)->bool {
			return !lstrcmp(pImageKey, Pair.first);
		});

	if (iter != m_mapBit.end()) {
		return (*iter).second->Get_MemDC();
	}
	//MessageBox(g_hWnd, L"실패", pImageKey, MB_OK);
	return nullptr;
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(),
		[pImageKey](auto& Pair)->bool {
			return !lstrcmp(pImageKey, Pair.first);
		});

	if (iter != m_mapBit.end()) {
		MessageBox(g_hWnd, L"실패", pImageKey, MB_OK);
		return;
	}
	else {
		CBmp* newBmp = new CBmp;
		newBmp->Load_Bmp(pFilePath);
		m_mapBit.insert({ pImageKey,newBmp });
		//MessageBox(g_hWnd, L"Success", pImageKey, MB_OK);ㅋ
	}
}
