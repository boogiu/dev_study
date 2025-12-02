#include "Client_Defines.h"
#include "NpcState_Interact_SmallTalk.h"
#include "NonPlayer.h"
#include "EventSystem.h"
#include "Animator3D.h"
#include "Player.h"
#include "NpcState_Interact_TransItem.h"

CNpcState_Interact_SmallTalk::CNpcState_Interact_SmallTalk()
{
}

HRESULT CNpcState_Interact_SmallTalk::OnEnter()
{

    /*토킹 시작했다. 이벤트 전달.*/
    /*잠깐, 플레이어가 먼저 시작한 이벤트라면? 굳이 보내줄 필요 없다. -> 이벤트 수신에서 처리*/
    OnStartDialogue event{ 
        EVENT_TYPE::DialougueStart,
        m_pCharacter->Get_TracePack().pPlayer,
        m_pCharacter,
        m_pCharacter->Get_EventPack().ConsumeSequence(),
        m_pCharacter->Get_EventPack().externalCondition,//=>플레이어가 말을 걸었다면 이걸 바꿔줄 것임
        m_pCharacter->Get_NpcData().NpcName
    };
    m_pCharacter->Get_EventPack().eventSystem->OnBroadCast<BaseEvent>(event);

     auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
     Animator->Change_Animation("Base_Wait.anim", true);
     m_pCharacter->Get_TracePack().pPlayer->Get_InfoPack().isTalking = true;

     m_pCharacter->LookTo(
         m_pCharacter->Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos()
     );
    return S_OK;
}

void CNpcState_Interact_SmallTalk::OnUpdate(_float dt)
{
    m_pCharacter->LookTo(
        m_pCharacter->Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos()
    );
}

HRESULT CNpcState_Interact_SmallTalk::OnExit()
{
    auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
    Animator->Change_Animation("Base_Wa`it.anim", true);

    return S_OK;
}

CState* CNpcState_Interact_SmallTalk::HandleTransition()
{
    return nullptr;
}

CNpcState_Interact_SmallTalk* CNpcState_Interact_SmallTalk::Create()
{
    return new CNpcState_Interact_SmallTalk;
}

void CNpcState_Interact_SmallTalk::Free()
{
    __super::Free();
}
