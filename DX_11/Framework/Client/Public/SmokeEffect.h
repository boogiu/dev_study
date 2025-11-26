#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CSmokeEffect :
    public CBaseEffect
{
   
private:
    CSmokeEffect();
    CSmokeEffect(const CSmokeEffect& rhs);
    virtual ~CSmokeEffect() override DEFAULT;

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
    _float m_fSpeed = {};
    _float m_fBaseSpeed = {};
    vector<CBaseEffect*> m_Clouds;
public:
    static CSmokeEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
