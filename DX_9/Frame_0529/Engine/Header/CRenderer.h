#pragma once

#include "CComponent.h"
BEGIN(Engine)

class CTransform;
class CMesh;

class ENGINE_DLL CRenderer :
    public CComponent
{
public:
    explicit CRenderer();
    virtual ~CRenderer() override;

public:
    static CRenderer* Create();

public:
    HRESULT Ready_Component() override;
    void Update_Component(float dt) override;
    void LateUpdate_Component(float dt) override;
    void Render(LPDIRECT3DDEVICE9 pDevice);
    CComponent* Clone() const override;

public:
    static COM_TYPE Get_StaticType() { return COM_TYPE::RENDERER; }
    COM_TYPE Get_Type() override { return Get_StaticType(); }
    void Set_Mesh();
private:
    void Free() override;

private:
    CTransform* m_pTransform;
    CMesh* m_pMesh;
};
END
