#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CStars :
    public CGameObject
{
private:
    CStars();
    CStars(const CStars& rhs);
    virtual ~CStars()DEFAULT;

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
    _uint m_ParicleCount = {};
    _float m_ElapsedTime = {};
    vector<INSTANCE_INIT_DESC>m_InitDescs;
    vector<INSTANCE_PARTICLE> m_Particles;

public:
    static CStars* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};
NS_END

