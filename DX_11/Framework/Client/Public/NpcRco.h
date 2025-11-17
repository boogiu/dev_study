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
    void Awake()override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;
    virtual void Set_Closed(OnEndDialogue endMsg)override;

public:
    virtual void Serve_Order(const string& order, _uint orderer) override;
    virtual void Receive_QuestMsg(QUEST_MSG msg)override;
    virtual void EventAction(const BaseEvent& event) override;

public:
    static CNpcRco* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
