#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CInstanceMesh: public CBase
{
	typedef struct tagINSTANCEInitDESC{
		class CVIBuffer* pBaseBuffer = { nullptr };
		_uint instanceStride = {};		// 인스턴스 구조체 크기 
		_uint instanceCount = {};     // 최대 인스턴스 개수
	}INSTANCE_INIT_DESC;

protected:
	CInstanceMesh(const string& bufferID);
	CInstanceMesh(const CInstanceMesh& rhs);
	virtual ~CInstanceMesh();

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, INSTANCE_INIT_DESC initDesc);
	HRESULT Update_Instance(ID3D11DeviceContext* pContext, const void* pData, _uint count);
public:
	HRESULT Bind_Buffer(ID3D11DeviceContext* pContext);
	HRESULT Render(ID3D11DeviceContext* pContext);

private:
	ID3D11Buffer* m_pInstanceBuffer = {nullptr};
	class CVIBuffer* m_pBaseBuffer = { nullptr };
	INSTANCE_INIT_DESC m_Desc = {};
	_uint m_iActiveCount = {};
public:
	static CInstanceMesh* Create(ID3D11Device* pDevice, const string& bufferID, INSTANCE_INIT_DESC initDesc);
	virtual void Free() override;
};

NS_END