#pragma once
#include "CRenderer.h"

BEGIN(Engine)

class CTerrain;
class CTexture;

class ENGINE_DLL CTerrainRenderer :
    public CRenderer
{
private:
   explicit CTerrainRenderer();
  virtual  ~CTerrainRenderer();

public:
    static CTerrainRenderer* Create();

public:
    HRESULT Ready_Component() override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;

public:
    void Set_Mesh(const string& key);
    void Set_Transform();
    void  Set_Terrain(int xSize, int zSize, int CellSize, float scale);

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    CTerrain* m_pMesh;
  
private:
    void Free() override;
};

END