#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CPlayerPart_Hand :
    public CGameObject
{
private:
    CPlayerPart_Hand();
    CPlayerPart_Hand(const CPlayerPart_Hand& rhs);
    virtual ~CPlayerPart_Hand() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void Change_Item(ITEM_DATA_DESC data);
    void Active_ColliderTool(_bool Active, string Event);
public:
    ITEM_TYPE Get_CurrentItemType() { return m_eItemType; };

private:
    ITEM_TYPE m_eItemType = { ITEM_TYPE::NONE };
    class CToolItem* m_pToolItem = { nullptr };

public:
    static CPlayerPart_Hand* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END