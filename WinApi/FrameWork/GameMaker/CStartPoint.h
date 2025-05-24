#pragma once
#include "CTile.h"
class CStartPoint :
    public CTile
{
public:
    CStartPoint();
    CStartPoint(TILE_INFO _info);
    ~CStartPoint();

public:
    virtual void Set_Tile(TILE_INFO _info) override;
};

