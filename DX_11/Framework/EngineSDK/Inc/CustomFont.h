#pragma once
#include "Base.h"
NS_BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont();
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice,  const wstring& FontFilePath);
	void Draw(SpriteBatch* pBatch, wstring pText, const _float2& vPosition, _fvector vColor,
		_float rotation, const _float2& origin, _float scale);
	
	_vector TextSize(wstring text, _bool ignorWhite = false) { return m_pFont->MeasureString(text.c_str(), ignorWhite); }
	_float LineSpace() { return m_pFont->GetLineSpacing(); }
private:
	SpriteFont* m_pFont = { nullptr };

public:
	static CCustomFont* Create(ID3D11Device* pDevice,  const wstring& FontFilePath);
	virtual void Free() override;
};

NS_END