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
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg) override;

public:
   void Render_Card(ID3D11DeviceContext* pContext);
public:
    void Hover();

private:
    void Ready_Part();
private:
    _bool   m_bActive = { false };
    _bool   m_bHover = { false };

    class CTarget_Texture* m_pIconImage = { nullptr };
    class CTarget_Texture* m_pBaseBackGround = { nullptr };
    class CTarget_Texture* m_pMaskedBackGround = { nullptr };

    _float2 vBaseSize = {};
    _float fBaseRadian = {};

public:
    static CUI_ItemCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END