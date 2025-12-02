#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CScreenFX :
    public CUI_Object
{
private:
    CScreenFX();
    CScreenFX(const CScreenFX& rhs);
    ~CScreenFX() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg) override;
    void Render_GUI() override;

private:
    _bool IsActive = {};
    _float m_fLifeTime = {1.f};

public:
    static CScreenFX* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END