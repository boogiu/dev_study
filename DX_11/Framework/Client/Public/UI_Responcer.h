#pragma once
#include "LevelObject.h"
NS_BEGIN(Client)
class CUI_Responcer :
    public CLevelObject
{
private:
    CUI_Responcer();
    CUI_Responcer(const CUI_Responcer& rhs);
    ~CUI_Responcer() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt)override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Active_UI(string uiName,void* pArg = nullptr);
    void DeActive_UI(string uiName,void* pArg = nullptr);
public:
    void Render_GUI() override;

private:
    unordered_map<string,CUI_Object*> m_UIContainer;

public:
    static CUI_Responcer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END