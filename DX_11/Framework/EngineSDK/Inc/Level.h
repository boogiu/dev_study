#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CLevel :
    public CBase
{
protected:
    CLevel();
    virtual ~CLevel() DEFAULT;

public:
    virtual HRESULT Initiailze();
    virtual void Update(_float dt);
    virtual HRESULT Render();

public:
    virtual void Free() override;
};

NS_END