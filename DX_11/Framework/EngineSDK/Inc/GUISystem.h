#pragma once
#include "IGUIService.h"
#include "GUIWidget.h"

NS_BEGIN(Engine)

class CGUISystem :
    public IGUIService
{
private:
    CGUISystem();
   virtual  ~CGUISystem() DEFAULT;

public:
    HRESULT Initialize(const ENGINE_DESC& engine, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Update(_float& dt);
    virtual void GUI_Begin();
    virtual void Render_GUI();
    virtual void GUI_End();
public:
    virtual bool Set_ProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual _bool UsingUI() { return m_bUsingUI; }

private:
    void Test();
private:
    bool m_bUsingUI;

public:
    static CGUISystem* Create(const ENGINE_DESC& engine, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;

};

NS_END