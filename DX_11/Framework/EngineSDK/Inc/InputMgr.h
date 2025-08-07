#pragma once
#include "IInputService.h"

NS_BEGIN(Engine)

class CInputMgr final :
    public IInputService
{
private:
    explicit CInputMgr();
    virtual ~CInputMgr();
    virtual HRESULT Initialize(HWND hwnd);
public:
    virtual void Update();

public:
    _bool Key_Tap(int vk_Key) { return m_Keyboard[vk_Key].state == KEY_STATE::TAP; }
    _bool Key_Hold(int vk_Key) { return m_Keyboard[vk_Key].state == KEY_STATE::HOLD; }
    _bool Key_Away(int vk_Key) { return m_Keyboard[vk_Key].state == KEY_STATE::AWAY; }
    _bool Key_Down(int vk_Key) { return Key_Tap(vk_Key) || Key_Hold(vk_Key); }

    _bool Mouse_Tap(MOUSE_BTN btn) { return m_Mouse.mouseKey[static_cast<int>(btn)].state == KEY_STATE::TAP; };
    _bool Mouse_Hold(MOUSE_BTN btn) { return m_Mouse.mouseKey[static_cast<int>(btn)].state == KEY_STATE::HOLD; };
    _bool Mouse_Away(MOUSE_BTN btn) { return m_Mouse.mouseKey[static_cast<int>(btn)].state == KEY_STATE::AWAY; };
    _bool Mouse_Down(MOUSE_BTN btn) { return Mouse_Tap(btn) || Mouse_Hold(btn); }

    const _float& Mouse_DeltaX() { return m_Mouse.fDeltaX; };
    const _float& Mouse_DeltaY() { return m_Mouse.fDeltaY; };
    const _float& Mouse_DeltaW() { return m_Mouse.fWheelDelta; };

    void Process_Input(LPARAM lParam);
private:
    void Process_RawInput(RAWINPUT* raw);

private:
    HWND m_hWnd = nullptr;
    vector<BYTE> m_vecInputMsg;
    UINT messageSize = 0;

    vector<KEY_DESC> m_Keyboard;
    MOUSE_DESC m_Mouse;

private:
    void HandleKeyboardInput(const RAWKEYBOARD& keyboard);
    void HandleMouseInput(const RAWMOUSE& mouse);

public:
    static CInputMgr* Create(HWND hwnd);
    virtual void Free() override;
};
NS_END
