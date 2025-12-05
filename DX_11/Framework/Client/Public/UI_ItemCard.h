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
    void Set_Index(_float2 rowCol,_float2 cardIndex, _float2 cardSize,_float2 TargetSize) {
        m_CardIndex = cardIndex;
        vBaseSize = cardSize;
        m_TargetSize = TargetSize;
        m_ColRow = rowCol;
        Ready_Part();
    }
public:
   void Render_Card(ID3D11DeviceContext* pContext);
   class CTarget_Texture* Make_Part(string textureKey, const  _float2& size, const  _float2& pos,const string& passKey,const string& levelKey);
public:
    _bool Hover();
    void UnHover() { m_bHover = false; };
    void Set_Data(const string& recipeImage, _bool CanCraft, wstring name);
    wstring Get_Data();
private:
    void Ready_Part();

private:
    _bool   m_bActive = { false };
    _bool   m_bHover = { false };

    wstring name = {};
    _bool CanCraft = { false };

    _float2 m_ColRow = {};
    _float2 m_CardIndex = {};
    _float2 m_CardSize = {};
    _float2 m_TargetSize = {};
    class CTarget_Texture* m_pIconImage = { nullptr };
    class CTarget_Texture* m_pBaseBackGround = { nullptr };
    class CTarget_Texture* m_pMaskedBackGround = { nullptr };

    _float2 vBaseSize = {};
    _float fBaseRadian = {};
    _float2 scale = _float2(1.f / m_ColRow.x, 1.f / m_ColRow.y);
    _float2 offset = _float2(1.f / m_ColRow.x, 1.f / m_ColRow.y);

public:
    static CUI_ItemCard* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END