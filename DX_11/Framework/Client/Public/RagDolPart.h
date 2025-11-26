#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CRagDolPart :
    public CBaseEffect
{
private:
    CRagDolPart();
    CRagDolPart(const CRagDolPart& rhs);
    virtual ~CRagDolPart() override DEFAULT;

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
    _float4 m_vBaseVelocity = {};
    _float4 m_vVelocity = {};

    _float m_fGravity = { 9.8f };
public:
    static CRagDolPart* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
