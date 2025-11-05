#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CInsect_Object :
    public CGameObject
{
    enum MoveState {STOP, RUN, CAPTURE, RUNAWAY, END};
protected:
    CInsect_Object();
    CInsect_Object(const CInsect_Object& rhs);
    virtual ~CInsect_Object() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;

public:
    virtual virtual void Render_GUI()override;

public:
    virtual virtual void Set_OwnMatrix(_float4x4* OwnMat) { m_pOwnerMatrix = OwnMat; };

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void MoveFly(_float dt);
    void MoveRunaway(_float dt);
    void Check_Player();
    void ReadyToDead();

private:
    _float m_fLifeTime = {};
    MoveState m_eState = { STOP };
    _float m_fDetectRange = {};
    _float4 m_vMoveDir = {};

    TILE_INDEX m_PrevIndex = {};
    TILE_INDEX m_CurrIndex = {};

    _float4x4* m_pOwnerMatrix = {nullptr};
public:
    static CInsect_Object* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END