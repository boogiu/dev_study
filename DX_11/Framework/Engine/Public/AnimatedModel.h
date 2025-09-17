#pragma once
#include "Model.h"
NS_BEGIN(Engine)
class ENGINE_DLL CAnimatedModel :
	public CModel
{
protected:
	CAnimatedModel();
	CAnimatedModel(const CAnimatedModel& rhs);
	virtual ~CAnimatedModel() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	virtual HRESULT Link_Buffer(const string& levelKey, const string& MeshKey);
	virtual  _uint Get_ElementCount() { return VTXSKINMESH::iElementCount; };
	virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() { return VTXSKINMESH::Elements; };
	HRESULT Render_Mesh(ID3D11DeviceContext* pContext, _uint Index) override;

public:
	void Update_Animation(_float dt);
	const vector<_float4x4>& Get_BoneMatrices(_uint DrawIndex);

protected:
	class CSkeleton* m_pSkeleton = { nullptr };
	vector<vector<_float4x4>>m_FinalBoneMatrices;

public:
	static CAnimatedModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};
NS_END