#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_EmptySlot :
    public CUI_Object
{
    enum SlotDataState { Empty, Filled };
    enum SlotUIState { Hovered, HoverOut, IDLE };
    struct InvenSlot {
        ITEM_DATA_DESC itemData = {};
        _uint itemCount = {};
    };
private:
    CUI_EmptySlot();
    CUI_EmptySlot(const CUI_EmptySlot& rhs);
    virtual ~CUI_EmptySlot() override;

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

private:
    void SizeControl(_float dt);

private:
    _float2 m_vBaseSize = { 15.f,15.f };
    _float2 m_vHoveredSize = { 30.f,30.f };
    SlotUIState m_eState = { IDLE };
    InvenSlot m_itemData = {};
public:
    static CUI_EmptySlot* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END