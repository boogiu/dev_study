#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CUI_ItemIcon :
    public CUI_Object
{
private:
    CUI_ItemIcon();
    CUI_ItemIcon(const CUI_ItemIcon& rhs);
    virtual ~CUI_ItemIcon() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    static CUI_ItemIcon* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END