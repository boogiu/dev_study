#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_InvenSlot :
    public CUI_Object
{
    enum SlotUIState { Hovered, HoverOut, IDLE };
    struct InvenSlot {
        ITEM_DATA_DESC itemData = {};
        _uint itemCount = {};
    };
private:
    CUI_InvenSlot();
    CUI_InvenSlot(const CUI_InvenSlot& rhs);
    virtual ~CUI_InvenSlot() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void isHovered() { m_eState = Hovered; };
    void isHoverOut() { m_eState = HoverOut; };
    _bool isAbleToContain(ITEM_DATA_DESC desc);
    _bool isItemFilled() { return m_itemData.itemCount != 0; };
    HRESULT Add_Data(ITEM_DATA_DESC desc);

private:
    void SizeControl(_float dt);

private:
    _float2 m_vBaseSize = { 20.f,20.f };
    _float2 m_vHoveredSize = { 40.f,40.f };
    SlotUIState m_eState = { IDLE };
    InvenSlot m_itemData = {};

    CUI_Object* m_pIcon = { nullptr };
    class CUI_ItemText* m_pText = { nullptr };
public:
    static CUI_InvenSlot* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END