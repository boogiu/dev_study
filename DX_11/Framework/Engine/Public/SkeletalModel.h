#pragma once
#include "Model.h"

NS_BEGIN(Engine)
class ENGINE_DLL CSkeletalModel :
	public CModel
{
	using BUFFER_MATRICES = vector<_float4x4>;

protected:
	CSkeletalModel();
	CSkeletalModel(const CSkeletalModel& rhs);
	virtual ~CSkeletalModel() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	virtual HRESULT Link_Model(const string& levelKey, const string& modelDataKey);
	HRESULT Draw(ID3D11DeviceContext* pContext, _uint Index) override;

public:
	virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc(_uint DrawIndex) override;
	virtual const _uint Get_ElementCount(_uint DrawIndex) override;
	virtual const string_view Get_ElementKey(_uint DrawIndex) override;
	virtual _uint Get_MeshCount() override;
	virtual _uint Get_MaterialIndex(_uint Index) override;
	virtual _bool isDrawable(_uint Index) override;

	class CModelData* Get_ModelData() { return m_pData; };
	virtual BOUNDING_BOX Get_LocalBoundingBox()	override;
	virtual BOUNDING_BOX Get_WorldBoundingBox()	override;
	virtual vector<BOUNDING_BOX> Get_MeshBoundingBox()	override;
	const vector<_float4x4>& Get_BoneMatrices() { return m_FinalMatices; };
	_bool isReadyToDraw()	override;

public:
	virtual void Render_GUI();

protected:
	vector<_bool> m_DrawableMeshes;
	vector<_float4x4> m_TransfromationMatrices = {};
	vector<_float4x4> m_CombinedMatrices = {};
	vector<_float4x4> m_FinalMatices = {};
	class CModelData* m_pData = { nullptr };

public:
	static CSkeletalModel* Create();
	virtual CComponent* Clone() override;
	virtual void Free() override;
};
NS_END