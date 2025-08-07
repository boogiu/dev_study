#pragma once
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CFont :
    public CBase
{
private:
    explicit CFont();
    virtual ~CFont();

public:
	HRESULT Ready_Font(const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);
	void Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color, DWORD Format);

private:
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXSPRITE				m_pSprite;		// 2d 텍스처
	LPD3DXFONT					m_pFont;		// 폰트 컴 객체

public:
	static CFont* Create(
		const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

private:
    virtual void Free();
};

END