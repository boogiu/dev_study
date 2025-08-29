#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CBasePanel abstract:
    public CBase
{
protected:
    CBasePanel();
    virtual ~CBasePanel() =default;

public:
    virtual void Render_GUI() PURE;
    _bool Get_Active() { return m_bActive; };
    void Set_Active(_bool Active) { m_bActive = Active; };

protected:
    _bool m_bActive = { true };
public:
    virtual void Free() override;
};
NS_END