#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_EventMsg :
    public CUI_Object
{

private:
    CUI_EventMsg();
    CUI_EventMsg(const CUI_EventMsg& rhs);
    virtual ~CUI_EventMsg() DEFAULT;

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
    vector<wstring> sequence;

    /*UI 종료 여부*/
    function<void()> m_onClose = {};
    class CUI_Cursor* m_pCursor = { nullptr };
   class CUI_Text* m_pTexts;

public:
    static CUI_EventMsg* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END