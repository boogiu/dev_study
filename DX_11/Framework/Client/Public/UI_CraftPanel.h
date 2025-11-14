#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_CraftPanel :
    public CUI_Object
{
    enum  UI_State {Opening,Opened,Selected,Closed };
private:
    CUI_CraftPanel();
    CUI_CraftPanel(const CUI_CraftPanel& rhs);
    virtual ~CUI_CraftPanel() DEFAULT;

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
    void Ready_Parts();
    _int Valid_Index(_int Add);


private:
    HRESULT Read_CraftData();

private:
    _bool   m_bActive = { false };
    UI_State m_eState = { Closed };

    _float xCardSize    ={};
    _float yCardSize={};

    _uint m_CardCount = {13};
    _int m_Col = {5};
    _int m_Row = {5};
    _int m_NowIndex = {};
    class CUI_Cursor* m_pCursor = { nullptr };
    class CCraftCard* m_pCraftCard= { nullptr };
    class CUI_ItemText* m_pText = { nullptr };

    vector<class CUI_ItemCard*> m_pCards;

    unordered_map<wstring, _uint> m_CraftData;
    unordered_map<wstring, _uint> m_InvenData;
    function<void(const CRAFT_RESULT& result)> m_OnClose;

public:
    static CUI_CraftPanel* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END