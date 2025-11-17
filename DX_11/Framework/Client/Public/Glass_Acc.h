#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CGlass_Acc :
    public CGameObject
{
private:
    CGlass_Acc();
    CGlass_Acc(const CGlass_Acc& rhs);
    virtual ~CGlass_Acc() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    static CGlass_Acc* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END