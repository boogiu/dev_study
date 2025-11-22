#pragma once
#include "Base.h"

class IEffectRenderer 
    : public CBase
{
public:
    virtual void RenderAll() PURE;
    virtual ~IEffectRenderer() = default;
};
