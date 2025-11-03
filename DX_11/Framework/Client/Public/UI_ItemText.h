#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CUI_ItemText :
    public CUI_Object
{
    enum state {Closed, Opened, Idle};
private:
    CUI_ItemText();
    CUI_ItemText(const CUI_ItemText& rhs);
    virtual ~CUI_ItemText() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Set_Text(wstring Text, _bool Active);
    void DeActive_Text();

private:
    wstring m_Text;
    state m_eState = { Closed };
    _float m_fOpenTime = {};
   _float2 m_BaseSize = { 85,42 };
public:
    static CUI_ItemText* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END