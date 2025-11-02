#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CUI_Cursor :
    public CUI_Object
{
private:
    CUI_Cursor();
    CUI_Cursor(const CUI_Cursor& rhs);
    virtual ~CUI_Cursor() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

private:

public:
    static CUI_Cursor* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END