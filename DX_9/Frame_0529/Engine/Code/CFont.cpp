#include "Engine_Define.h"
#include "CFont.h"
#include "CGraphicDev.h"
// 유니코드 ☺ 테스트

CFont::CFont()
{
}

CFont::~CFont()
{
}

HRESULT CFont::Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	m_pDevice = CGraphicDev::GetInstance()->Get_GraphicDev();
	m_pDevice->AddRef();

	D3DXFONT_DESCW			tFont_Desc;
	ZeroMemory(&tFont_Desc, sizeof(D3DXFONT_DESCW));

	tFont_Desc.CharSet = HANGEUL_CHARSET;
	tFont_Desc.Width = iWidth;
	tFont_Desc.Height = iHeight;
	tFont_Desc.Weight = iWeight;

	lstrcpyW(tFont_Desc.FaceName, pFontType);

	if (FAILED(D3DXCreateFontIndirectW(m_pDevice, &tFont_Desc, &m_pFont)))
	{
		::MessageBoxW(0, L"Font Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		::MessageBoxW(0, L"Sprite Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color, DWORD Format)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = (_long)pPos->x;//충분히 큰 너비
	rc.bottom = (_long)pPos->y;   // 충분히 큰 높이

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlenW(pString), &rc, Format, Color);

	m_pSprite->End();
}

CFont* CFont::Create(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont* pInstance = new CFont;

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		Safe_Release(pInstance);
		MessageBoxW(0, L"폰트 문제", L"Error", MB_OK);
		return nullptr;
	}

	return pInstance;
}

void CFont::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pDevice);
}
