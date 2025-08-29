#pragma once
#include "VIBuffer.h"
NS_BEGIN(Engine)
class ENGINE_DLL CVI_Rect final :
    public CVIBuffer
{
protected:
	CVI_Rect();
	CVI_Rect(const CVI_Rect& rhs);
	virtual ~CVI_Rect();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice)override;

private:
	virtual HRESULT Create_Vertex(ID3D11Device* pDevice) override;
	virtual HRESULT Create_Index(ID3D11Device* pDevice) override;

public:
	static CVI_Rect* Create(ID3D11Device* pDevice) ;
	virtual void Free() override;

	// CVIBuffer을(를) 통해 상속됨
	_uint Get_ElementCount() override;
	const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() override;
};
NS_END
