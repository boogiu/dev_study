#pragma once
#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ICameraService abstract :
public IService
{
protected:
    virtual  ~ICameraService() DEFAULT;
public:
    virtual void Update(_float dt) PURE;
    virtual void Set_MainCam(class CCamera* pCamCom) PURE;
    virtual const _float4x4* Get_ViewMatrix() PURE;
    virtual const _float4x4* Get_ProjMatrix() PURE;
    virtual const _float4x4* Get_InversedViewMatrix() PURE;
    virtual const _float4x4* Get_InversedProjMatrix() PURE;
    virtual const _float4 Get_CameraPos() PURE;
};
NS_END
