#pragma once
#include "PlayerState.h"
NS_BEGIN(Client)
class CPlayerState_TransTool :
    public CPlayerState
{
private:
    CPlayerState_TransTool();
    virtual ~CPlayerState_TransTool() DEFAULT;

public:
    virtual HRESULT OnEnter();
    virtual void OnUpdate(_float dt);
    virtual HRESULT OnExit();
    virtual CState* HandleTransition();

public:
    virtual void Render_State();
    virtual _uint Get_InputMask() const override;

private:
    _bool m_bTransed = { false };
public:
    static CPlayerState_TransTool* Create();
    virtual void Free();
};
NS_END
