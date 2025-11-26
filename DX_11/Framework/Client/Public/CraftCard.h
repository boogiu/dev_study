#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CCraftCard :
    public CUI_Object
{
    struct ResourceSlotsForCard {
        class CTarget_Texture* pSourceItem = { nullptr };
        class CTarget_Texture* pSourceItemLine = { nullptr };
        class CTarget_Texture* pSourceItemBLine= { nullptr };
        class CTarget_Texture* pSourceItemCountSlot= { nullptr };

        class CUI_Text* pRecipeItemName = { nullptr };
        class CUI_Text* pRecipeItemCount = { nullptr };
        class CUI_Text* pRecipeItemNeed = { nullptr };
        void Render(ID3D11DeviceContext* pContext);

        void DeActive();
        void Active();
    };
    struct BaseTemplete {
        class CTarget_Texture* pBaseBackGround = { nullptr };
        class CTarget_Texture* pImageBackGround = { nullptr };
        class CTarget_Texture* pInfoBackGround = { nullptr };
        class CTarget_Texture* pNameStroke = { nullptr };
        class CTarget_Texture*                 pButtonBG = { nullptr };
        class CTarget_Texture*                 pButton = { nullptr };
        class CTarget_Texture*                 pButtonEffect = { nullptr };
        class CUI_Text*                 pRecipeDial = { nullptr };

        void Render(ID3D11DeviceContext* pContext);
    };

private:
    CCraftCard();
    CCraftCard(const CCraftCard& rhs);
    virtual ~CCraftCard() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Render_CraftCard(ID3D11DeviceContext* pContext);

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg=nullptr) override;
    void Set_Panel(class CUI_CraftPanel* pPanel) { m_pPanel = pPanel; };
    void Set_Data(CraftData data);
private:
    void Ready_Part();
    class CTarget_Texture* MakePart(const string& texture, const _float2& size, const _float2& offset, const string& pass, const string& name);
    class CUI_Text* MakeText(const wstring& Text, const _float2& size, const _float& sclae, const _float2& offset, const _float4& color , const string& name);

private:
    _bool   m_bActive = { false };
    _float2 m_vTargetSize = {};
    _float2 m_vBasePos = {};
    _float m_fElpasedTime = {};

    _bool ButtonOn = { false };
    _float m_fEffectTime= {};

private:
    CraftData m_data = {};
    class CTarget_Texture* m_pItemImage = { nullptr };
    class CTarget_Texture* m_pCategoryIcon= { nullptr };
    class CUI_Text* m_pRecipeName= { nullptr };
    class CUI_CraftPanel* m_pPanel = { nullptr };
    class CUI_Cursor* m_pCursor = { nullptr };

    ResourceSlotsForCard m_Slot1 = {};
    ResourceSlotsForCard m_Slot2 = {};  
    ResourceSlotsForCard m_Slot3 = {};
    BaseTemplete m_Templete = {};
public:
    static CCraftCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END