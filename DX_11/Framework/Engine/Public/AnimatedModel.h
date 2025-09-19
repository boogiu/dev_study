#pragma once
#include "Model.h"
NS_BEGIN(Engine)
class ENGINE_DLL CAnimatedModel :
	public CModel
{
	using BUFFER_MATRICES = vector<_float4x4>;
protected:
	CAnimatedModel();
	CAnimatedModel(const CAnimatedModel& rhs);
	virtual ~CAnimatedModel() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	virtual HRESULT Link_Model(const string& levelKey, const string& MeshKey);
	HRESULT Render_Model(ID3D11DeviceContext* pContext, _uint Index) override;

public:
	virtual void Render_GUI();

public:
	static CAnimatedModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};
NS_END