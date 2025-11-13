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
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

private:
    void Ready_Part();
private:
    _bool   m_bActive = { false };


#ifdef _USING_GUI
    _uint childIndex = {};
#endif // _USING_GUI
public:
    static CCraftCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END