#pragma once
#include "ICameraService.h"
NS_BEGIN(Engine)
class CCameraMgr :
    public ICameraService
{
private:
    CCameraMgr();
    virtual ~CCameraMgr() DEFAULT;
public:
    virtual void Set_MainCam(class CCamera* pCamCom) override;
public:
    virtual void Update(_float dt) override;
    virtual const _float4x4* Get_ViewMatrix() override { return &m_ViewMatrix; };
   virtual const _float4x4* Get_ProjMatrix() override { return &m_ProjMatrix; };
   virtual const _float4x4* Get_InversedViewMatrix() override { return &m_InversedViewMatrix; };
   virtual const _float4x4* Get_InversedProjMatrix() override { return &m_InversedProjMatrix; };
   virtual const _float4 Get_CameraPos() override;
public:
    static CCameraMgr* Create();
    virtual void Free() override;
private:
    class CCamera* m_pMainCam = {nullptr};
    _float4x4 m_ViewMatrix = {};
    _float4x4 m_ProjMatrix = {};
    _float4x4 m_InversedViewMatrix = {};
    _float4x4 m_InversedProjMatrix = {};
    _float4 m_vCamPos = {};
};

NS_END