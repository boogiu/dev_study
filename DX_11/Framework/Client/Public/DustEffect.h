#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CDustEffect :
    public CBaseEffect
{
private:
    CDustEffect();
    CDustEffect(const CDustEffect& rhs);
    virtual ~CDustEffect() override DEFAULT;

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
    _float m_fRadian = {};
    _float m_fRawTime = {};
public:
    static CDustEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
