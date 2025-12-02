#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CLeafParticle :
    public CGameObject
{
private:
    CLeafParticle();
    CLeafParticle(const CLeafParticle& rhs);
    virtual ~CLeafParticle()DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Render_GUI() override;

public:

private:
    _uint m_ParticleCount = {};
    vector< INSTANCE_INIT_DESC> m_InitDescs;
    vector< INSTANCE_PARTICLE> m_Particle;
    vector<_float3> m_vVelocity;
public:
    static CLeafParticle* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};

NS_END