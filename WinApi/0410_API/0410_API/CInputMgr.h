#pragma once
#include "Define.h"
class CInputMgr :
    public Singleton<CInputMgr>
{
    friend class Singleton;
private:
    CInputMgr();
    ~CInputMgr();
public:
    void Initialize();
    void Render();
    void Update();
    void Release();
public:
    bool KeyDown   (int _vKey);
    bool KeyTap        (int _vKey);
    bool KeyHold      (int _vKey);
    bool KeyAway    (int _vKey);

private:
    vector<KEY_STATE> m_vecKey;
};

