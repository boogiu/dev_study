#pragma once
#include "NonPlayer.h"
NS_BEGIN(Client)
class CNpcRco :
    public CNonPlayer
{
private:
    CNpcRco();
    CNpcRco(const CNpcRco& rhs);
    virtual ~CNpcRco() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    //virtual void Render_GUI();

public:
    static CNpcRco* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
