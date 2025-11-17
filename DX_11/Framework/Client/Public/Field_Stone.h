#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CField_Stone :
    public CFieldObject
{
    enum  State
    {
        HITTED,
        IDLE,
        READY_TO_DESTROY,
        END,
    };

private:
    CField_Stone();
    CField_Stone(const CField_Stone& rhs);
    virtual ~CField_Stone() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();
    virtual HRESULT Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void Override_Pass();
private:
    void HittedMove(_float dt);

private:
    _float ItemSpawnCoolTime = {};
    State m_eState = {IDLE};
    _float4 m_vMoveVector = {};
    _float4 m_vHittedPos = {};
    _bool m_isJustHitted = { false };

    _uint m_HitCount = { 0 };
public:
    static CField_Stone* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END