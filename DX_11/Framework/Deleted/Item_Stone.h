#pragma once
#include "Item_Object.h"

NS_BEGIN(Client)
class CItem_Stone :
    public CItem_Object
{
private:
    CItem_Stone();
    CItem_Stone(const CItem_Stone& rhs);
    virtual ~CItem_Stone() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;
    virtual virtual void Render_GUI();

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

public:
    static CItem_Stone* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END