#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CMaterialInstance;
NS_END
NS_BEGIN(Client)

class CDustEffect :
    public CGameObject
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

private:
    _float m_ElapsedTime = {};
     CMaterialInstance* m_pInstance = { nullptr };

public:
    static CDustEffect* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
