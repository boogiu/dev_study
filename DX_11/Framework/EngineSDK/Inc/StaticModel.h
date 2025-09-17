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
    virtual HRESULT Link_Buffer(const string& levelKey, const string& MeshKey);
    virtual  _uint Get_ElementCount() { return VTXMESH::iElementCount; };
    virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() { return VTXMESH::Elements; };
    HRESULT Render_Mesh(ID3D11DeviceContext* pContext, _uint Index) override;

public:
    static CStaticModel* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END