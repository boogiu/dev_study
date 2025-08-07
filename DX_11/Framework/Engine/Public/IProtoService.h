#pragma once
#include "IService.h"
NS_BEGIN(Engine)

class ENGINE_DLL IProtoService abstract:
    public IService
{
protected:
    virtual ~IProtoService() override DEFAULT;
};

NS_END
