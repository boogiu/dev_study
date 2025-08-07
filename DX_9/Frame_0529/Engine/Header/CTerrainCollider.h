#pragma once
#include "CCollider.h"
BEGIN(Engine)

class CTerrain;

class ENGINE_DLL CTerrainCollider :
    public CCollider
{
private:
    explicit CTerrainCollider();
    virtual ~CTerrainCollider();
public:
    static CTerrainCollider* Create();
public:
    HRESULT Ready_Component() override;
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    CComponent* Clone() const override;
public:
    void Set_Mesh(const wstring& key);

private:
    CTerrain* m_pMesh;

private:
    void Free() override;

};

END