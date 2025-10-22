#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    typedef struct tagPlayerPartsDesc : GAMEOBJECT_DESC {
        CGameObject* pPlayer = { nullptr };
    }PLAYER_PARTS_DESC;

private:
    CPlayer();
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    _float2 Get_InputAxis() { return m_vInputAxis; };
    _float Get_MoveSpeed() { return m_fMoveSpeed; };
    ITEM_TYPE Get_CurrentItemType();
private:
    void Add_AnimationClips();
    void Add_PartObjects();

private:
    class CPlayerStateMachine* m_pStateMachine= { nullptr };
    _float2 m_vInputAxis = {};
    _float  m_fMoveSpeed = {15.f};
public:
    static CPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
