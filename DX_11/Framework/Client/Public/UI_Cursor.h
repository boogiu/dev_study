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

public :
    void Set_Pivot(_float2 Pivot, _float2 Offset = { 25,25 },_float2 Movevec = {1,1});
private:
    _float2 m_vPivot = {};
    _float m_fFloatingTime = {  };
    _float m_fFloatingSpeed = { 10.f};


public:
    static CUI_Cursor* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END