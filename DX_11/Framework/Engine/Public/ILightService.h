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
	virtual _int Register_Light(class CLight* Light) PURE;
	virtual void UnRegister_Light(_int ID) PURE;
};
NS_END
