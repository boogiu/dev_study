#include "Client_Defines.h"
#include "PlayerState_Walk.h"
#include    "Player.h"
#include "Animator3D.h"
#include "Transform.h"
#include "PlayerState_Movement.h"
#include "GameInstance.h"
#include "IInputService.h"

CPlayerState_Walk::CPlayerState_Walk()
{
}

void CPlayerState_Walk::OnEnter()
{
    auto Animator = m_pPlayer->Get_Component<CAnimator3D>();
    HRESULT hr =Animator->Chane_Animation("Move_Run_F.anim");
    if (FAILED(hr)) {
        int i = 0;
    }
}

void CPlayerState_Walk::OnUpdate(_float dt)
{
    CPlayerState_Movement* Parent =  static_cast<CPlayerState_Movement*>(m_pParent);
  
    if (!Parent->isFliping()) {
        _float2 InputAxis = m_pPlayer->Get_InputAxis();
        _float MoveSpeed = m_pPlayer->Get_MoveSpeed();
        InputAxis.y *= MoveSpeed * dt;
        InputAxis.x *= MoveSpeed * dt;

        _float4 pos = m_pPlayer->Get_Position();
        pos.x += InputAxis.x;
        pos.z += InputAxis.y;
        if (Parent->CheckMovable({ pos.x, pos.y, pos.z,0.f })) {
            CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
            pTransform->Set_Pos({ pos.x, pos.y, pos.z });
        }
    }

    _float TurnDegree = Parent->Get_CurrentDegree();
    CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
    pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(TurnDegree));
}

void CPlayerState_Walk::OnExit()
{
}

CState* CPlayerState_Walk::HandleTransition()
{
    _float2 InputAxis = m_pPlayer->Get_InputAxis();
    auto Animator = m_pPlayer->Get_Component<CAnimator3D>();

    if (fabs(InputAxis.x) == 0 && fabs(InputAxis.y)==0) {
        Animator->Chane_Animation("ToStop_RunLatter_L.anim",0.2f);

        if(Animator->isCurrentAnimEnd())
            return m_pHFSM->Get_State("Movement_Idle_State");
    }
    else if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SHIFT)) {
        return m_pHFSM->Get_State("Movement_Run_State");
    }
    else {
        HRESULT hr = Animator->Chane_Animation("Move_Run_F.anim");
    }
    return nullptr;
}

CPlayerState_Walk* CPlayerState_Walk::Create()
{
    return new CPlayerState_Walk;
}

void CPlayerState_Walk::Free()
{
}
