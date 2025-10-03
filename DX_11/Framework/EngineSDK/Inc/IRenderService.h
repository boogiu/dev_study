#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL IRenderService abstract:
    public IService
{
protected:
    virtual  ~IRenderService() DEFAULT;
public:
    virtual HRESULT Render() PURE;
    virtual void Submit_Opaque(const OPAQUE_PACKET& packet) PURE;
    virtual void Submit_UI(const UI_PACKET& packet) PURE;
    virtual void Submit_Debug(const DEBUG_PACKET& packet) PURE;
};
NS_END
