#pragma once
#include "NonPlayer.h"
NS_BEGIN(Client)
class CNpcRcm :
    public CNonPlayer
{
private:
    CNpcRcm();
    CNpcRcm(const CNpcRcm& rhs);
    virtual ~CNpcRcm() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;
    virtual void Set_Closed(OnEndDialogue endMsg)override;

public:
    virtual void Serve_Order(const string& order, _uint orderer) override;
    virtual void EventAction(const BaseEvent& event) override;

public:
    static CNpcRcm* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
