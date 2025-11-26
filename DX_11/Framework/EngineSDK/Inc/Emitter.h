#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CEmitter :
    public CBase
{
protected:
    CEmitter();
    ~CEmitter() DEFAULT;
public:
    virtual void Update(_float dt, const EffectTransform& transform) PURE;
    virtual void Render() PURE;
public:
    virtual void Free();
};

NS_END