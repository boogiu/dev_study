#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CTerrain :
    public CGameObject
{
private:
    CTerrain();
    CTerrain(const CTerrain& rhs);
    virtual ~CTerrain() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

private:

public:
    static CTerrain* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END