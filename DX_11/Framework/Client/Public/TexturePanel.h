#pragma once
#include "UI_Object.h"
NS_BEGIN(Client)
class CTexturePanel :
    public CUI_Object
{
private:
    CTexturePanel();
    CTexturePanel(const CTexturePanel& rhs);
    virtual ~CTexturePanel() override;

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

public:
    static CTexturePanel* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
