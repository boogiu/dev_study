#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CTextSlot :
    public CComponent
{
    struct AnchorInfo {
        _bool bAutoPos = { false };
        ANCHOR eAnchor;
        _float2 vPivot;
    };
private:
    CTextSlot();
    CTextSlot(const CTextSlot& rhs);
    virtual ~CTextSlot() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    HRESULT Set_Font(const string FontTag);
    void Set_Color(_float4 color);
    void Set_Size(_float scale);
    void Set_Position(_float2 Pos);
    void Set_Text(wstring Text) { m_Info.Text = Text; };

public:
    void Push_Text();
    void Set_AutoPos(ANCHOR anchor, _float2 Pivot);
    void Release_AutoPos() {
        m_AnchorInfo.bAutoPos = false;
    }

public:
    void Set_Anchor(ANCHOR anchot, _float2 Pivot);
    _float2 Get_Anchor(ANCHOR anchot);

private:
    class CCustomFont* m_pFont = { nullptr };
    TEXT_INFO m_Info = { L"",{0,0},{1,1,1,1},"",1.f,0.f,{0.f,0.f} };
    AnchorInfo m_AnchorInfo = {};

public:
    static CTextSlot* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END