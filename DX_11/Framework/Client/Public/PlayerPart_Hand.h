#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CPlayerPart_Hand :
    public CGameObject
{
public:
    typedef struct tagCharacterPartsDesc : GAMEOBJECT_DESC {
        CGameObject* pOwner = { nullptr };
        string BoneName = { };
    }CHARACTER_PARTS_DESC;

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
    void OnCollisionEnter(COLLISION_CONTEXT context);
    void OnCollisionStay(COLLISION_CONTEXT context);
    void OnCollisionExit(COLLISION_CONTEXT context);

public:
    void Change_Item(TOOL_DATA_DESC data);
    void Active_ColliderTool(_bool Active, string Event);
    void Active_ColliderHand(_bool Active, string Event);

public:
    itemType Get_CurrentItemType() { return m_eItemType; };

private:
    itemType m_eItemType = { itemType::None };
    class CToolItem* m_pToolItem = { nullptr };
    CGameObject* m_pOwner = { nullptr };
    string m_OwnerBone = {};
public:
    static CPlayerPart_Hand* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END