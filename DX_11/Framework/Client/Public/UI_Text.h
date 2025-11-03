#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CUI_Text :
    public CUI_Object
{
private:
    CUI_Text();
    CUI_Text(const CUI_Text& rhs);
    virtual ~CUI_Text() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;
public:
    void Set_Active(_bool active) { m_bActive = active; }
    void Set_Anchor(ANCHOR eAnchor) { m_eAnchor = eAnchor; }

private:
    _bool m_bActive = { false };
    ANCHOR m_eAnchor = { ANCHOR::Center};

public:
    static CUI_Text* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
