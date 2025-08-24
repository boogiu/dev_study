#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CLevel abstract:
    public CBase
{
protected:
    CLevel();
    virtual ~CLevel() DEFAULT;

public:
    virtual HRESULT Initialize() PURE;
    virtual void Update() PURE;
    virtual HRESULT Render() PURE;

public:
    virtual void Free() override;
};

NS_END