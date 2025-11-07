#pragma once
#include "NpcState.h"
NS_BEGIN(Client)
class CNpcState_Move_Walk :
    public CNpcState
{
    private:
        CNpcState_Move_Walk();
        virtual ~CNpcState_Move_Walk() DEFAULT;

    public:
        virtual HRESULT OnEnter() override;
        virtual void OnUpdate(_float dt) override;
        virtual HRESULT OnExit() override;
        virtual CState* HandleTransition() override;

private:
    _float m_fWalkDuration = {  };
    public:
        static CNpcState_Move_Walk* Create();
        virtual void Free() override;
};

NS_END