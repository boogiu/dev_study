#pragma once
#include "UI_Object.h"

NS_BEGIN(Client)
class CTarget_Text :
    public CUI_Object
{
private:
    CTarget_Text();
    CTarget_Text(const CTarget_Text& rhs);
    virtual ~CTarget_Text() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Render(ID3D11DeviceContext* pContext);

public:
    virtual void UI_Active(void* pArg) override;
    virtual void UI_DeActive(void* pArg) override;
public:
    _float Text_Length();
    void Set_Text(const wstring& text);
    void Clear_Text();
    void Set_Color(_float4 color);
    void Set_Scale(_float size);
    void Rotate(_float radian);

public:
    void TargetSize(_float2 size);
    void Center(_float2 offset = {});
    _float2 Target_Center(_float2 offset = { });
    _float Target_L(_float offset = { });
    _float Target_R(_float offset = { });
    _float Target_B(_float offset = { });
    _float Target_T(_float offset = { });
private:
    _float4x4 m_OrthoProject = {};
public:
    static CTarget_Text* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END