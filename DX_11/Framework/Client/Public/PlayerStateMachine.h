#pragma once
#include "Base.h"
#include "Player.h"

NS_BEGIN(Client)

#pragma region STATE_MACHINE
class CPlayerStateMachine :
    public CBase
{
private:
    CPlayerStateMachine(CPlayer* pPlayer);
    virtual ~CPlayerStateMachine() DEFAULT;

public:
    HRESULT Initialize();

public:
    void Update(_float dt);
public:
    void Render_State(class CPlayer* pPlayer);
private:
    CPlayer* m_pOwner = { nullptr };
    class CPlayerHFSM* m_pHFSM = { nullptr };
public:
    static CPlayerStateMachine* Create(CPlayer* pPlayer); 
    virtual void Free();
};
#pragma endregion

NS_END