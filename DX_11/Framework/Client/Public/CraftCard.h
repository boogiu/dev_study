#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CCraftCard :
    public CUI_Object
{

private:
    CCraftCard();
    CCraftCard(const CCraftCard& rhs);
    virtual ~CCraftCard() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Render_CraftCard(ID3D11DeviceContext* pContext);

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg=nullptr) override;

private:
    void Ready_Part();


private:
    _bool   m_bActive = { false };
    _float2 m_vTargetSize = {};
    _float2 m_vBasePos = {};
private:
    class CTarget_Texture* m_pBaseBackGround = { nullptr };
    class CTarget_Texture* m_pImageBackGround = { nullptr };
    class CTarget_Texture* m_pInfoBackGround = { nullptr };
    class CTarget_Texture* m_pItemImage = { nullptr };
    class CTarget_Texture* m_pNameStroke= { nullptr };
    class CTarget_Texture* m_pCategoryIcon= { nullptr };

    class CTarget_Texture* m_pSourceItem_01= { nullptr };
    class CTarget_Texture* m_pSourceItemLine_01= { nullptr };
    class CTarget_Texture* m_pSourceItemBLine_01= { nullptr };
    class CTarget_Texture* m_pSourceItemCountSlot= { nullptr };

public:
    static CCraftCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END