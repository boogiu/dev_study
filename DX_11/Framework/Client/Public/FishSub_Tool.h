#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CAnimator3D;
NS_END

NS_BEGIN(Client)
class CFishSub_Tool :
    public CGameObject
{
private:
    CFishSub_Tool();
    CFishSub_Tool(const CFishSub_Tool& rhs);
    virtual ~CFishSub_Tool() override;

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

public:
    void Sync_Bont_To_Rod(class CAnimator3D* pAnimator, const string& boneName);

private:
    _bool m_bAttached = { false };
    _float3 m_vBaseOffset = {0,0,0};
    _float3 m_vCurrentOffset = {};

public:
    static CFishSub_Tool* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
