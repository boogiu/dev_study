#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CToolItem :
    public CGameObject
{
public:
    typedef struct tagPlayerToolDesc : GAMEOBJECT_DESC {
        CGameObject* pOwner = { nullptr };
    }CHRACTER_TOOL_DESC;

private:
    CToolItem();
    CToolItem(const CToolItem& rhs);
    virtual ~CToolItem() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context) override;
    void OnCollisionExit(COLLISION_CONTEXT context) override;

private:
    void AdjustByItem(itemType type);
public:
    void Set_Item(TOOL_DATA_DESC data);

private:
    CGameObject* m_pOwner = { nullptr };
    _uint m_SubIndex = {};

public:
    static CToolItem* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END