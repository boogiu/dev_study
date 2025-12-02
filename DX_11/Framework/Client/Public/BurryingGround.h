#pragma once
#include "BaseEffect.h"
NS_BEGIN(Client)
class CBurryingGround :
    public CBaseEffect
{
private:
    CBurryingGround();
    CBurryingGround(const CBurryingGround& rhs);
    virtual ~CBurryingGround() override DEFAULT;

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
    vector<class CBaseEffect*> m_Parts;
    CBaseEffect* m_pSmoke = { nullptr };
    _float m_fTimeCheck = {};
    _int m_NowIndex = {};
    _bool m_bEnd_Bury = { false };
public:
    static CBurryingGround* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
