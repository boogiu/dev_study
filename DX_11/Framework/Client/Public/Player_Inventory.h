#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CPlayer_Inventory :
    public CUI_Object
{
    enum InvenState {Openning, Opened, Selected ,Closing, Closed};
public:
    typedef struct tagCharacterPartsDesc : UI_DESC {
        CGameObject* pOwner = { nullptr };
    }CHARACTER_PARTS_DESC;

private:
    CPlayer_Inventory();
    CPlayer_Inventory(const CPlayer_Inventory& rhs);
    virtual ~CPlayer_Inventory() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Set_Player(class CPlayer* pPlayer);
    void Open_Inventory();
    void Close_Inventory();
    HRESULT Add_ItemToInventory(ITEM_DATA_DESC desc);
    HRESULT PullOut_Item(_int Slot);
    HRESULT PullOut_ToOtherSlot(_int Slot);

private:
    void Batch_Slots();
    void DeActive_Slots();
    void Openning_Inven(_float dt);
    void Closing_Inven(_float dt);

private:
    void Pointing_Item(_float dt);
    void Select_Item(_float dt);
    vector<wstring> Switch_ItemSelect(itemType type, _uint count);

private:
    class  CPlayer* m_pPlayer = { nullptr };

private:
    InvenState m_eState = {Closed};
       _int nowIndex = {};
       vector<class CUI_InvenSlot*> m_pSlots;
    class CUI_Cursor* m_pCursor = { nullptr };
    class CSelectPanel* m_pSelectPanel = { nullptr };

    _float4 m_vTimer = {  0,0,0,0  };
    _float2 m_vOpenSize = {  450,150  };
    _float2 m_vCloseSize = { };
    _float2 m_vOpenPos = {  1280 / 2,250  };
    _float2 m_vPointPos = { 1280 / 2.4,290 };
    _float2 m_vClosePos = { 1280 / 2,300 };

 
    _int prevIndex = {-1};
public:
    static CPlayer_Inventory* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END