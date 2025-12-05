#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CMoon :
    public CGameObject
{
private:
    CMoon();
    CMoon(const CMoon& rhs);
    virtual ~CMoon()DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Render_GUI() override;
private:
    void Update_Bounce(_float dt);
    _float EaseIn(_float time);
private:
    _float m_ElapsedTime = {};
public:
    static CMoon* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};
NS_END