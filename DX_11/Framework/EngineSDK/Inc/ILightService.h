#pragma once
#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL ILightService abstract :
public IService
{
protected:
	virtual  ~ILightService() DEFAULT;
public:
	virtual void Register_Light(LIGHT_DESC* Light) PURE;
	virtual void UnRegister_Light(LIGHT_DESC* Light) PURE;
};
NS_END
