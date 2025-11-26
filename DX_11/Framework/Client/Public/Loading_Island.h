#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CLoading_Island :
    public CUI_Object
{
private:
    CLoading_Island();
    CLoading_Island(const CLoading_Island& rhs);
    virtual ~CLoading_Island() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg) override;

private:
    _float m_fLifeTime = {};
public:
    static CLoading_Island* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
