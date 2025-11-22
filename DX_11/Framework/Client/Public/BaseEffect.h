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

protected:
    _float m_fLifeTime = {};
    CMaterialInstance* m_pMaterial_Instance = { nullptr };

public:
    void Free() override;
};

NS_END
