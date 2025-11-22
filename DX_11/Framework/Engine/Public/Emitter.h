#pragma once
#include "Base.h"

struct Particle
{
    _float3 vPosition;
    _float3 vVelocity;
    _float  fLife;
    _float  fMaxLife;
};

NS_BEGIN(Engine)
class CEmitter :
    public CBase
{
protected:
    CEmitter();
    ~CEmitter() DEFAULT;
public:
    virtual HRESULT Initialize(EmitterTemplate* pData) PURE;
    virtual void Update(_float dt, const EffectTransform& transform) PURE;
    virtual void Render() PURE;
public:
    virtual void Free();
};

NS_END