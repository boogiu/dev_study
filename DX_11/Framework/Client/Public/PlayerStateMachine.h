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
    void EvaluateTransition(); 
    void SetInput(const _float2& moveAxis);
    _float2 GetInputAxis() { return m_vInputAxis;};
public:
    string GetStateName(CPlayer::Player_State eState);
    void Render_StateGUI(CPlayer* pPlayer);
private:
    CPlayer* m_pOwner = { nullptr };
    CPlayer::Player_State m_eNowState = {};
    CPlayer::Player_State m_eNextState = {};

    _float2 m_vInputAxis = {};
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
    virtual void Render_StateGUI(CPlayer* pPlayer);
    virtual void Enter(CPlayer* pPlayer) PURE;
    virtual void Update(CPlayer* pPlayer, _float dt) PURE;
    virtual _bool Exit(CPlayer* pPlayer, _float dt) PURE;
    virtual _bool ReadyToExit(CPlayer* pPlayer) PURE;
};
#pragma endregion

#pragma region IDLE_STATE
class Player_IdleState :
    public IPlayerState
{
private:
    Player_IdleState(CPlayerStateMachine* pMachine);
    virtual ~Player_IdleState() DEFAULT;

public:
    virtual void Enter(CPlayer* pPlayer) override;
    virtual void Update(CPlayer* pPlayer, _float dt) override;
    virtual _bool Exit(CPlayer* pPlayer, _float dt) override;
    virtual _bool ReadyToExit(CPlayer* pPlayer) override;

private:
    CPlayerStateMachine* m_pMachine = { nullptr };
    _bool m_isEndIdle = { false };

public:
    static Player_IdleState* Create(CPlayerStateMachine* pMachine);
};
#pragma endregion

#pragma region MOVE_STATE
class Player_MoveState :
    public IPlayerState
{
private:
    Player_MoveState(CPlayerStateMachine* pMachine);
    virtual ~Player_MoveState() DEFAULT;

public:
    virtual void Enter(CPlayer* pPlayer) override;
    virtual void Update(CPlayer* pPlayer, _float dt) override;
    virtual _bool Exit(CPlayer* pPlayer, _float dt) override;
    virtual _bool ReadyToExit(CPlayer* pPlayer) override;

public:
    void Render_StateGUI(CPlayer* pPlayer)override;

private:
    CPlayerStateMachine* m_pMachine = { nullptr };
    _bool isFlipping = { false };
    _bool m_isMoveEnd = { false };
    _float m_fNowDegree= {};
    _float m_fMoveSpeed = { 10 };

public:
    static Player_MoveState* Create(CPlayerStateMachine* pMachine);
};
#pragma endregion

NS_END