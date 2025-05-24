#pragma once
#include "../MapEditor/CTile.h"
class COrnament :
    public CTile
{
public:
    COrnament();
    COrnament(int Col, int Row);
    COrnament(TILE_INFO _info);
    ~COrnament();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();
};

