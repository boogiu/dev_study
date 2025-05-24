#pragma once
#include "CObject.h"
class CTile :
    public CObject
{
public: 
    CTile();
    CTile(TILE_INFO _info);
   ~ CTile();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
public:
    virtual void Set_Tile(TILE_INFO _info);
    TILE_TYPE Get_Type() { return m_tTile.eType; }
    TILE_INFO Get_Tile_Info() { return m_tTile; }
    MAP_OBJ_TYPE Get_Obj_Type() { return m_eMapType; }
protected:
    MAP_OBJ_TYPE m_eMapType;
    TILE_INFO m_tTile;
};

