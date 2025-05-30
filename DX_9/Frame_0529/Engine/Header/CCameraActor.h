#pragma once
#include "CGameObject.h"
BEGIN(Engine)
class CTransform;
class CCamera;

class CCameraActor :
    public CGameObject
{
private:
    explicit CCameraActor();
    virtual ~CCameraActor();

public:
    static CCameraActor* Create();
    HRESULT Ready_CameraActor();
    void Update_Camera(float dt);
    void LateUpdate_Camera(float dt);
public:

private :
    CTransform* m_pTransform;
    CCamera* m_pCamera;
private:
    virtual void Free();
};

END