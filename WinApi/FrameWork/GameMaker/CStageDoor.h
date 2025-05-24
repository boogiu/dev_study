#pragma once
#include "CTile.h"
class CStageDoor :
    public CTile
{
public:
    CStageDoor();
    CStageDoor(TILE_INFO _info);
    ~CStageDoor();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    virtual void Set_Tile(TILE_INFO _info) override;
    void Open_Door() { m_bOpen = true; }
    bool Next_Stage() { return m_bNext;}
public:
    bool m_bNext;
    bool m_bOpen;
};

