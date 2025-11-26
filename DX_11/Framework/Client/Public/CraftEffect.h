#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CCraftEffect :
    public CBaseEffect
{
    enum phase { phase1, phase2, phase3, lastPhase,End };
private:
    CCraftEffect();
    CCraftEffect(const CCraftEffect& rhs);
    virtual ~CCraftEffect() override DEFAULT;

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
    void Emit_Particle(_float count, _bool isEnd = false);
    void Emit_Twinkle();

private:
    _float m_fRadian = {};
    _float m_fAtlasFrame = {};
    _float2 m_AtlasIndex = {};
    _float2 m_AtlasScale = {};
  phase m_eState = { phase1 };
    vector<class CCraftChildEffect*> m_Childs;
    vector<class CCraftTwinkle * > m_Twinckles;
public:
    static CCraftEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
