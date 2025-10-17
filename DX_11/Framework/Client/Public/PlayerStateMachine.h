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
    void ChangeState(CPlayer::Player_State eNext);

private:
    CPlayer* m_pOwner = { nullptr };
    CPlayer::Player_State m_eNowState = {};
    CPlayer::Player_State m_eNextState = {};

    unordered_map < CPlayer::Player_State, class IPlayerState*> m_StateContainer;

public:
    static CPlayerStateMachine* Create(CPlayer* pPlayer); 
    virtual void Free();
};
#pragma endregion

#pragma region STATE_INTERFACE
class IPlayerState :
    public CBase
{
protected:
    IPlayerState();
    virtual ~IPlayerState() DEFAULT;

public:
    virtual void Enter(CPlayer* pPlayer) PURE;
    virtual void Update(CPlayer* pPlayer, _float dt) PURE;
    virtual void Exit(CPlayer* pPlayer) PURE;
};
#pragma endregion

#pragma region IDLE_STATE
class Player_IdleState :
    public IPlayerState
{
private:
    Player_IdleState(CPlayerStateMachine* pOwner);
    virtual ~Player_IdleState() DEFAULT;

public:
    virtual void Enter(CPlayer* pPlayer) override;
    virtual void Update(CPlayer* pPlayer, _float dt) override;
    virtual void Exit(CPlayer* pPlayer) override;

private:
    CPlayerStateMachine* m_pOwner = { nullptr };

public:
    static Player_IdleState* Create(CPlayerStateMachine* pOwner);
};
#pragma endregion

#pragma region MOVE_STATE
class Player_MoveState :
    public IPlayerState
{
private:
    Player_MoveState(CPlayerStateMachine* pOwner);
    virtual ~Player_MoveState() DEFAULT;

public:
    virtual void Enter(CPlayer* pPlayer) override;
    virtual void Update(CPlayer* pPlayer, _float dt) override;
    virtual void Exit(CPlayer* pPlayer) override;

private:
    CPlayerStateMachine* m_pOwner = { nullptr };

public:
    static Player_MoveState* Create(CPlayerStateMachine* pOwner);
};
#pragma endregion

NS_END