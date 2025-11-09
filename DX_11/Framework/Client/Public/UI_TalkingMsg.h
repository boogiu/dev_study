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

private:
    _bool   m_bActive = { false };
    _bool m_bOpenComplete = { false };

    _float2 m_vOpenSize = {};
    _float   m_fOpenSpeed = {};

    /*텍스트 애님*/
    _bool m_bSequenceComplete = { false };
    _uint m_iSequence = {};
    _uint m_iSequenceSize = {};
    _float m_fTypeTime = {};
    _float m_fPauseTime = {};
    _float m_iPauseSubset = {};

    wstring speakerName = {  };
    vector<wstring> sequence;
    vector<function<void()>> Action;
    _bool m_bActionExecuted = false;
    /*UI 종료 여부*/
    function<void()> m_onClose = {};
    class CUI_Cursor* m_pCursor = { nullptr };
    class CUI_Text* m_pTexts = { nullptr };
    class CUI_NameTag* m_pNameTag = { nullptr };

public:
    static CUI_TalkingMsg* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END