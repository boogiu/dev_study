#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)

class CCraftTwinkle :
    public CBaseEffect
{
private:
    CCraftTwinkle();
    CCraftTwinkle(const CCraftTwinkle& rhs);
    virtual ~CCraftTwinkle() override DEFAULT;

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
    _float m_fFrameTime = {};

    _float2 m_AtlasIndex = {};
    _float2 m_AtlasScale = {};
    _float m_fAtlasFrame = {};

public:
    static CCraftTwinkle* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END