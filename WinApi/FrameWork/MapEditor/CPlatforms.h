#pragma once
#include "../MapEditor/CTile.h"

class CPlatforms :
    public CTile
{
public:
    CPlatforms();
    CPlatforms(TILE_INFO _info);
    CPlatforms(int Col, int Row);

    ~CPlatforms();

public  :
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();
};

