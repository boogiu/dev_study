#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
class CUI_Transition :
    public CLevelObject
{
private:
    CUI_Transition();
    CUI_Transition(const CUI_Transition& rhs);
    ~CUI_Transition() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Render_TransCraft(ID3D11DeviceContext* pContext);
    void Set_Active();
    void Set_DeActive();

public:
    void Render_GUI() override;

private:
    _bool m_bUp = {};
    _bool m_bDown = {};
    _float m_fLifeTime = {};

public:
    static CUI_Transition* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END