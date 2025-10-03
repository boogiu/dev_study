#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CDebugRender :
    public CComponent
{
private:
    CDebugRender();
    CDebugRender(const CDebugRender& rhs);
    virtual ~CDebugRender() DEFAULT;
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    HRESULT Render_DebugBox(ID3D11DeviceContext* pContext,class CModel* pModel );
    void Set_DebugBounding(const BOUNDING_BOX& box);
private:
    ID3D11Buffer* m_pVB = { nullptr };
    ID3D11Buffer* m_pIB = { nullptr };

public:
    static CDebugRender* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END