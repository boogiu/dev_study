#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CSceneCamera :
    public CGameObject
{
    enum state {NONE, START, LIVE};
private:
    CSceneCamera();
    CSceneCamera(const CSceneCamera& rhs);
    ~CSceneCamera() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Execute();

private:
    state m_eState = { NONE };
public:
    static CSceneCamera* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};

NS_END