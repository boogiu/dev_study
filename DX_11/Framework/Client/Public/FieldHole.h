#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldHole :
    public CGameObject
{
    enum HoleState{Digged, BarriedNothing, BarriedSomeThing, Ready_Delete, IDLE};
private:
    CFieldHole();
    CFieldHole(const CFieldHole& rhs);
    ~CFieldHole() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;
public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
protected:
    HoleState m_eState = { Digged };
    _float m_fLifeTime = { 0.f };
    TILE_INDEX m_SyncedIndex = {};
    //_bool Barried = { false };
public:
    static CFieldHole* Create();
    void Free() override;

    // CGameObject을(를) 통해 상속됨
    CGameObject* Clone(INIT_DESC* pArg) override;
};

NS_END

