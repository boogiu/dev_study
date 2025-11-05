#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CDialoguePanel :
    public CUI_Object
{
private:
    CDialoguePanel();
    CDialoguePanel(const CDialoguePanel& rhs);
    virtual ~CDialoguePanel() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

private:
    _bool m_bActive = { false };
    vector<class CUI_Text*>m_pTexts;
public:
    static CDialoguePanel* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
