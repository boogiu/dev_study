#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CUI_ItemCard :
    public CUI_Object
{
private:
    CUI_ItemCard();
    CUI_ItemCard(const CUI_ItemCard& rhs);
    virtual ~CUI_ItemCard() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Hover();
private:
    _bool   m_bActive = { false };
    _bool   m_bHover = { false };

    CUI_Object* m_pInside = { nullptr };
    CUI_Object* m_pImage = { nullptr };
    _float2 vBaseSize = {};
    _float vBaseRotation = {};

public:
    static CUI_ItemCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END