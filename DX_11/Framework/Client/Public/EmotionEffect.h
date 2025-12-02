#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CEmotionEffect :
    public CBaseEffect
{
private:
    CEmotionEffect();
    CEmotionEffect(const CEmotionEffect& rhs);
    virtual ~CEmotionEffect() override DEFAULT;

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
    void Convert_ByData(const EffectData& data);
private:
    _float m_fRadian = {};
    _float m_fAtlasFrame = {};
    _float2 m_AtlasIndex = {};
    _float2 m_AtlasScale = {};
    _float2 m_AtlasCount= {};
    _int   m_AtlasMaxFrame = 0;

    _float3 m_vDstScale = {};
public:
    static CEmotionEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
