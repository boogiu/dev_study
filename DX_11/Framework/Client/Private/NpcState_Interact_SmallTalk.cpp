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
    TALKING_EVENT event{ m_pCharacter,nullptr, "Player"};
    m_pCharacter->Get_EventPack().eventSystem->OnBroadCast(event);

     auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
     Animator->Change_Animation("Base_Wait.anim", true);

     TalkingMsgDesc desc = Make_EvtDesc();
    m_pCharacter->Open_Dialogue("TalkingMsg", &desc);
   m_pCharacter->LookTo(m_pCharacter->Get_TracePack().pPlayer->Get_Component<CTransform>()->Get_Pos());
    return S_OK;
}

void CNpcState_Interact_SmallTalk::OnUpdate(_float dt)
{
}

HRESULT CNpcState_Interact_SmallTalk::OnExit()
{
    auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
    Animator->Change_Animation("Base_Wait.anim", true);

    return S_OK;
}

CState* CNpcState_Interact_SmallTalk::HandleTransition()
{
    return nullptr;
}

TalkingMsgDesc CNpcState_Interact_SmallTalk::Make_EvtDesc()
{
    TalkingMsgDesc desc = {};
    desc.OpenSize = { 800,160 };
    desc.OpenSpeed = 8.f;
    desc.SpeakerID = m_pCharacter->Get_NpcData().NpcID;
    desc.startSequence = m_pCharacter->Get_EventPack().Ready_SequenceID;
    desc.Speaker = m_pCharacter;

    desc.OnClose = [this](_bool isEnd) {OnClose(isEnd); };
    return desc;
}

void CNpcState_Interact_SmallTalk::OnClose(_bool isEnd)
{
    if(isEnd){
    m_pCharacter->Get_EventPack().HasAgenda = false;
    m_pCharacter->Get_TracePack().pPlayer->Get_InfoPack().m_pTalker = nullptr;
    }
}

CNpcState_Interact_SmallTalk* CNpcState_Interact_SmallTalk::Create()
{
    return new CNpcState_Interact_SmallTalk;
}

void CNpcState_Interact_SmallTalk::Free()
{
    __super::Free();
}
