#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel abstract :
	public CComponent
{
protected:
	CModel();
	CModel(const CModel& rhs);
	virtual ~CModel() DEFAULT;

public:
	virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc(_uint DrawIndex) PURE;
	virtual const _uint Get_ElementCount(_uint DrawIndex) PURE;
	virtual const string_view Get_ElementKey(_uint DrawIndex) PURE;
	virtual HRESULT Draw(ID3D11DeviceContext* pContext, _uint Index) PURE;
	virtual HRESULT Link_Model(const string& levelKey, const string& modelDataKey)PURE;

public:
	virtual _uint Get_MeshCount() PURE;
	virtual _uint Get_MaterialIndex(_uint Index) PURE;
	virtual _bool isDrawable(_uint Index) PURE;

	virtual BOUNDING_BOX Get_LocalBoundingBox()	PURE;
	virtual _bool isReadyToDraw()	PURE;
public:
	virtual void Free() override;
};

NS_END