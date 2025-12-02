#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)
class CEndingController :
    public CLevelObject
{
private:
    CEndingController();
    CEndingController(const CEndingController& rhs);
    ~CEndingController() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Set_EventSystem(class CEventSystem* pSystem);
    void Recieve_Ending(const BaseEvent& evt);

public:
    void Batch_Character();
    void Execute_Scene();
    void Set_EnvCam();
private:
    _bool m_bEndingStart = { false };
    _float m_fElapsedTime = {};

    class CEventSystem* m_pEventSystem = { nullptr };
    class CPlayer* m_Player = { nullptr };
    vector<class CNonPlayer*> m_NonPlayers ;
public:
    static CEndingController* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
