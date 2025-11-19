#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CAnimator3D;
NS_END

NS_BEGIN(Client)
class CFishSub_Tool :
    public CGameObject
{
    enum state {NonActive,Attach,Throw, Flow,Hit, ReAttached};
private:
    CFishSub_Tool();
    CFishSub_Tool(const CFishSub_Tool& rhs);
    virtual ~CFishSub_Tool() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void Get_Event(const BaseEvent& event);

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context) override;
    void OnCollisionExit(COLLISION_CONTEXT context) override;

    void Set_Owner(CGameObject* pOwner) {  m_pOwner = pOwner;}

public:
    void Sync_Bont_To_Rod(_float4x4* pOwnerMatrix);
    void Missed();

private:
    void FollowBone(_float dt);
    void ThrowBey(_float dt);
    void FlowBey(_float dt);
    void ReturnToBone(_float dt);
private:

    state m_eState = { NonActive };
    /*Throw*/
    _float m_fThrowingTime = {};
    _float4 m_vDirection= {};
    _float4 m_vStartPos= {};
    _float4 m_vTargetPos= {};
    _float4 m_vVelocity= {};

    /*Flow*/
    _float m_fFloatingTime = {};
    _float m_fReturnTime = {};

    class CFish_Object* m_pTarget = { nullptr };
    _float4x4* m_pOwnerMatrix = { nullptr };
    CGameObject* m_pOwner = { nullptr };
    _float4* m_vLeftHand = {};

public:
    static CFishSub_Tool* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
