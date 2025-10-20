#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CHairParts :
    public CGameObject
{
public:

private:
    CHairParts();
    CHairParts(const CHairParts& rhs);
    virtual ~CHairParts() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    static CHairParts* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END