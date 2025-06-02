#pragma once
#include "CGameObject.h"
BEGIN(Engine)
class CTransform;
class CCamera;

class ENGINE_DLL CCameraActor :
    public CGameObject
{
private:
    explicit CCameraActor();
    virtual ~CCameraActor();

public:
    static CCameraActor* Create();
public:
    HRESULT Ready_GameObject() override;
    void Update_GameObject(float dt) override;
    void LateUpdate_GameObject(float dt) override;
public:
    CCamera* Get_Camera() { return m_pCamera; }

private :
    CTransform* m_pTransform;
    CCamera* m_pCamera;
private:
    virtual void Free();

};

END