#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CSelectPanel :
    public CUI_Object
{
private:
    CSelectPanel();
    CSelectPanel(const CSelectPanel& rhs);
    virtual ~CSelectPanel() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

private:
    vector<class CUI_ItemText*>m_pTexts;

public:
    static CSelectPanel* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
