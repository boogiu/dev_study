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

    if (m_pPlayer->Get_CurrentItemType() == ITEM_TYPE::AXE) {
        Animator->Set_AnimationBlend("ToolAxe_APose.anim", { 19,20,21,22,23,24,25,26,27,28,29,30,31 });
    }
}

void CPlayerState_Walk::OnUpdate(_float dt)
{
    CPlayerState_Movement* Parent =  static_cast<CPlayerState_Movement*>(m_pParent);
    _float2 Player_InputAxis = m_pPlayer->Get_InputAxis();
    isFlipping = Parent->isFliping();
    if (!isFlipping) {
        _float MoveSpeed = m_pPlayer->Get_MoveSpeed();
        _float2 myAxis = {};
        myAxis.x =Player_InputAxis.x *MoveSpeed *dt;
        myAxis.y =Player_InputAxis.y * MoveSpeed *dt;

        Parent->CheckMovable(myAxis);
        CTransform* pTransform = m_pPlayer->Get_Component<CTransform>();
        pTransform->Translate({ myAxis.x ,0,myAxis.y });
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

    if (!isFlipping&&fabs(InputAxis.x) == 0 && fabs(InputAxis.y)==0) {
        Animator->Chane_Animation("ToStop_RunLatter_L.anim");

        if (Animator->isCurrentAnimEnd()) {
            Animator->Reset_AnimationBlend();
            return m_pHFSM->Get_State("Idle_Base_State");
        }
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
