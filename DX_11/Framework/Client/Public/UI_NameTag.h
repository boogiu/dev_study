#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CUI_NameTag :
    public CUI_Object
{
private:
    CUI_NameTag();
    CUI_NameTag(const CUI_NameTag& rhs);
    virtual ~CUI_NameTag() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Set_Active(_bool active);
    void Set_Name(const wstring& name);

private:
    _bool m_bActive = { false };    
    class CUI_Text* m_pText = { nullptr };

public:
    static CUI_NameTag* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
