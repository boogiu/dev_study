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
public:
    void Active();
    void DeActive();
    void Set_Selecte(vector<wstring> select,_float dt);
    _int Check_Select() { return m_SelectedIndex; };
private:
    _bool m_bActive = { false };
    _uint m_SelectCount = {};
    _uint m_NowIndex = {};
    _int m_SelectedIndex = {-1};
    class CUI_Cursor* m_pCursor = { nullptr };
    class CUI_Object* m_pSelectHighlight = { nullptr };
    vector<class CUI_Text*>m_pTexts;
    _float MaxWidth = {};
public:
    static CSelectPanel* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
