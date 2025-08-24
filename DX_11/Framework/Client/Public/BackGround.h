#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CBackGround final:
    public CGameObject
{
private:
    CBackGround();
    CBackGround(const CBackGround& rhs);
    virtual ~CBackGround() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

private:

public:
    static CBackGround* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END