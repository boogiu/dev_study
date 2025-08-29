#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CVIBuffer abstract:public CBase
{
public:
	typedef struct tagVIBufferLoadDesc  : public INIT_DESC{
		BUFFER_TYPE m_eType;
	}VI_LOAD_DESC;

protected:
	CVIBuffer();
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice);
	virtual HRESULT Bind_Buffer(ID3D11DeviceContext* pContext);
	virtual HRESULT Render(ID3D11DeviceContext* pContext);

	virtual _uint Get_ElementCount() PURE;
	virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() PURE;

	virtual const string& Get_Key() { return m_VIKey; }
	virtual void Set_Key(const string& key) { m_VIKey = key; }

protected:
	virtual HRESULT Create_Vertex(ID3D11Device* pDevice);
	virtual HRESULT Create_Index(ID3D11Device* pDevice);

protected:
	ID3D11Buffer* m_pVB = {nullptr};
	ID3D11Buffer* m_pIB = { nullptr };

	/*버텍스 버퍼의 개수(버퍼 개수)*/
	_uint m_iVertexBufferCount = {};

	/*버텍스의 개수*/
	_uint m_iVerticesCount = {};
	_uint m_iVertexStride = {};

	_uint m_iIndicesCount = {};
	_uint m_iIndexStride = {};

	DXGI_FORMAT m_eIndexFormat = {};
	D3D_PRIMITIVE_TOPOLOGY m_ePrimitive = {};

	string m_VIKey;

public:
	virtual void Free() override;
};
NS_END
