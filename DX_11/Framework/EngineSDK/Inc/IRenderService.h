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
    virtual void Submit_Shadow(const OPAQUE_PACKET& packet) PURE;
    virtual void Submit_Shadow(const INSTANCE_PACKET& packet) PURE;
    virtual void Submit_Instance(const INSTANCE_PACKET& packet) PURE;
    virtual void Submit_UI(const UI_PACKET& packet) PURE;
    virtual void Submit_Debug(const DEBUG_PACKET& packet) PURE;
    virtual HRESULT Add_Palette(const string& ConstantName, class CTexture* pTexture) PURE;
#ifdef _USING_GUI
    virtual void Render_GUI() PURE;
#endif // _USING_GUI
    };
NS_END
