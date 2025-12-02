#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CFlowerEffect :
    public CBaseEffect
{
private:
    CFlowerEffect();
    CFlowerEffect(const CFlowerEffect& rhs);
    virtual ~CFlowerEffect() override DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;
public:
    virtual  _bool isEffectActive() override;
    virtual  void Reset() override;
    virtual  void Set_DeActive()override;
    virtual  void Set_ReActive(const EffectData& data)override;
private:
    _uint m_ParicleCount = {};
    vector<INSTANCE_PARTICLE> m_Particle;
    vector< _float3 > m_vVelocities;
    vector<INSTANCE_INIT_DESC> m_InitDescs;
public:
    static CFlowerEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
