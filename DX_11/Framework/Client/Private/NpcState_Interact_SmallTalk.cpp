#include "Client_Defines.h"
#include "NpcState_Interact_SmallTalk.h"
#include "NonPlayer.h"
#include "EventSystem.h"
#include "Animator3D.h"
#include "Player.h"

CNpcState_Interact_SmallTalk::CNpcState_Interact_SmallTalk()
{
}

HRESULT CNpcState_Interact_SmallTalk::OnEnter()
{
    TALKING_EVENT event{ m_pCharacter, "Player"};
    m_pCharacter->Get_EventPack().eventSystem->OnBroadCast(event);

    auto Animator = m_pCharacter->Get_Component<CAnimator3D>();
    Animator->Change_Animation("Base_Wait.anim", true);

    TalkingMsgDesc desc = {};
    desc.OpenSize = { 600,150 };
    desc.OpenSpeed = 8.f;
    desc.textSequence = { L"¾È³ç ºÎ±â!",L"ÀßÁö³Â¾î?"};
    auto Greeting = [Animator]() {
        Animator->Change_Animation("MaRe_Clapping.anim", false);
    };
    auto Idle = [Animator]() {
        Animator->Change_Animation("Base_Wait.anim", true);
        };
    desc.SequenceActing = { Idle,Greeting };
    desc.Speaker = L"³Ê±¼";
    
    desc.OnClose = [this]() {OnClose(); };

    m_pCharacter->Open_Dialogue("TalkingMsg", &desc);
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

TalkingMsgDesc CNpcState_Interact_SmallTalk::Make_Desc()
{
    return TalkingMsgDesc();
}

void CNpcState_Interact_SmallTalk::OnClose()
{
    m_pCharacter->Get_EventPack().HasAgenda = false;
    m_pCharacter->Get_TracePack().pPlayer->Get_InfoPack().m_pTalker = nullptr;
}

CNpcState_Interact_SmallTalk* CNpcState_Interact_SmallTalk::Create()
{
    return new CNpcState_Interact_SmallTalk;
}

void CNpcState_Interact_SmallTalk::Free()
{
    __super::Free();
}
