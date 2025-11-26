#pragma once
#include "BaseEffect.h"

NS_BEGIN(Client)
class CCraftChildEffect :
    public CBaseEffect
{

private:
    CCraftChildEffect();
    CCraftChildEffect(const CCraftChildEffect& rhs);
    virtual ~CCraftChildEffect() override DEFAULT;

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

public:
    void Set_Dir(_float3 dir, _float duration,_uint Index);

private:
    _float m_fDuration = {};
    _float3 m_vMoveDir = {};

    _float m_fRadian = {};
public:
    static CCraftChildEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END