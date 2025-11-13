#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_TalkingMsg :
    public CUI_Object
{

private:
    CUI_TalkingMsg();
    CUI_TalkingMsg(const CUI_TalkingMsg& rhs);
    virtual ~CUI_TalkingMsg() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg) override;

private:
    void SequenceClear();
    void Ready_Parts();
    void DoSeqMotions();
    void DoPostAction();
    void Active_Select();
    void ReArrange_Sequence(_int selectChoice);

private:
    _bool   m_bActive = { false };
    _bool m_bOpenComplete = { false };

    _float2 m_vOpenSize = {};
    _float   m_fOpenSpeed = {};
    _float   m_fElapseTime = {};

    class CUI_Cursor* m_pCursor = { nullptr };
    class CUI_Text* m_pTexts = { nullptr };
    class CUI_NameTag* m_pNameTag = { nullptr };
    class CSelectPanel* m_pSelectPanel = { nullptr };
    class CDialogueManager* m_pDialogueManager = { nullptr };


    /*던져지는 정보들*/
    _int m_SpeakerID = {};
    wstring m_SpeakerName = {};
    class CNonPlayer* m_pSpeaker = { nullptr };
    class CPlayer* m_pListner = { nullptr };

    vector<SEQUENCE_DATA_DESC> m_Sequences;
    function<void(OnEndDialogue)> m_onClose = {};
    
    _int m_nowSeqIndex = {};

    /*UI 애님 -> 타이핑 애니메이션*/
    _float m_fTypingTime = {};
    _float m_fTypePuaseTime = {};
    _int m_iTypeSubStr = {};

    /*하나의 대사 출력이 끝났는가*/
    _bool m_bSeqComplete = { false };
    /*정해진 모션이 호출 되었는가*/
    _bool   m_bMotionCalled = { false };
    /*시퀀스 선택지가 존재하는가*/
    _bool   m_bHasSelection = { false };
    _bool   m_bSelectionActive = { false };


public:
    static CUI_TalkingMsg* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END