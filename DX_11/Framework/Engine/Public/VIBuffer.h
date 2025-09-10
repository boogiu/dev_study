#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CVIBuffer abstract:public CBase
{
protected:
	CVIBuffer(const string& key);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice);
	virtual HRESULT Bind_Buffer(ID3D11DeviceContext* pContext);
	virtual HRESULT Render(ID3D11DeviceContext* pContext);

	const string& Get_Key() { return m_VIKey; }
	void Set_Key(const string& key) { m_VIKey = key; }

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
