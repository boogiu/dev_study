#pragma once
#include "IService.h"
NS_BEGIN(Engine)

class ENGINE_DLL IGUIService :
    public IService
{
protected:
    virtual ~IGUIService() DEFAULT;

public:
    virtual void Update(_float& dt) PURE;
    virtual void GUI_Begin() PURE;
    virtual void Render_GUI() PURE;
    virtual void GUI_End()PURE;
    virtual bool Set_ProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)PURE;
    virtual _bool UsingUI()PURE;
};

NS_END