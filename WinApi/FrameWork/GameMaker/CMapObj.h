#pragma once
#include "../GameMaker/CTile.h"
class CRader;

class CMapObj :
    public CTile
{
public:
    CMapObj();
    CMapObj(TILE_INFO _info);
    ~CMapObj();
public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _hDC) override;
    void Release() override;
public:
    MAP_OBJ_TYPE Get_ObjType() { return m_eObjType; }
    virtual void Set_Tile(TILE_INFO _info) override;

private : 
    void Set_ObjType();
private:
    MAP_OBJ_TYPE m_eObjType;
    CRader* m_pRader;
};

