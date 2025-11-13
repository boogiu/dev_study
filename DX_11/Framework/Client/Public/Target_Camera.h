#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CTarget_Camera :
    public CGameObject
{
    enum CamState {NONE, FOLLOW, ZOOM_IN, ZOOM_OUT,TALKING,TALK_OUT, SHAKE};
public:
    typedef struct tagTargetCamDesc : GAMEOBJECT_DESC {
        CGameObject* pTarget = { nullptr };
        _float4 vOffset = {};
    }TARGET_CAM_DESC;

private:
    CTarget_Camera();
    CTarget_Camera(const CTarget_Camera& rhs);
    virtual ~CTarget_Camera()DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Execute_ZoomIn();
    void Release_ZoomIn();

public:
    void Execute_Talking(CGameObject* subject);
    void Release_Talking(CGameObject* subject);

private:
    void Zoom_In(_float dt);
    void Zoom_Out(_float dt);
    void Zoom_Talking(_float dt);
    void Zoom_TalkingOut(_float dt);
    void Follow_Target(_float dt);
    void Shake_Cam(_float dt);

public:
    void Render_GUI() override;
private:
    _float4 m_vOffset= {};
    _float4 m_vZoomInOffset= {};
    _float4 m_vLookPos= {};

    CamState m_eState = {FOLLOW};
    CGameObject* m_pTarget = { nullptr };
    CGameObject* m_pSubject = { nullptr };

    _float m_fCurrentLookY = {};
    _float4 m_vBasePos = {};
    _float4 m_vBaseLookPos = {};

    _float m_fShakeTime = {};
    _float4 m_vShakeOffset = {};
    _float4 m_vShakeBasePos = {};
    _float3 m_vShakePhase = {};
    CamState m_prevState = { FOLLOW };

public:
    static CTarget_Camera* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};

NS_END