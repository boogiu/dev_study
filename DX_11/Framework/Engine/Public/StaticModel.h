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
    virtual HRESULT Link_Model(const string& levelKey, const string& MeshKey);
    HRESULT Render_Model(ID3D11DeviceContext* pContext, _uint Index) override;

public:
    static CStaticModel* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END