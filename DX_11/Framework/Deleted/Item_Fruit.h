#pragma once
#include "Item_Object.h"

NS_BEGIN(Client)
class CItem_Fruit :
    public CItem_Object
{
private:
    CItem_Fruit();
    CItem_Fruit(const CItem_Fruit& rhs);
    virtual ~CItem_Fruit() DEFAULT;

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
    void Dangle_Fruit(const string& boneName, _float3 offset);

private:
    _float3 m_Offset = {};

public:
    static CItem_Fruit* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END