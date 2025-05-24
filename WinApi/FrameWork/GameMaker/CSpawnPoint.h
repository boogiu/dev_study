#pragma once
#include "CTile.h"

struct SQUAD {
    int soldier;
    int archor;
    int mage;
    int giant;
};

class CSpawnPoint :
    public CTile
{
public:
    CSpawnPoint();
    CSpawnPoint(TILE_INFO _info);
   ~CSpawnPoint();
public: 
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();
public:
    virtual void Set_Tile(TILE_INFO _info) override;
private:
    void Spawn();
private:
    bool m_bSpawn;
    CObject* myEnemy;
};

