#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CMaterialInstance;
NS_END

NS_BEGIN(Client)
class CBaseEffect :
    public CGameObject
{
protected:
    CBaseEffect();
    CBaseEffect(const CBaseEffect& rhs);
    virtual ~CBaseEffect() override DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;

public:
    virtual  _bool isEffectActive() { return false; };
    virtual  void Set_DeActive();
    virtual  void Set_ReActive(const EffectData& data);

    virtual  void Reset() {};
    string Get_Tag() { return m_EffectTag; }
    void Set_Tag(const string& tag) { m_EffectTag = tag; };

protected:
    _float m_fLifeTime = {};
    string m_EffectTag = {};
    CMaterialInstance* m_pMaterial_Instance = { nullptr };

public:
    static CBaseEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END
