 #pragma once
#include "Model.h"
NS_BEGIN(Engine)
class ENGINE_DLL CStaticModel :
    public CModel
{
protected:
    CStaticModel();
    CStaticModel(const CStaticModel& rhs);
    virtual ~CStaticModel() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;

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
    _bool isReadyToDraw()	override { return m_pData != nullptr; };

protected:
    vector<_bool> m_DrawableMeshes;
    class CModelData* m_pData = { nullptr };

public:
    static CStaticModel* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END