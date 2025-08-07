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
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;

public:
    void Set_Mesh(const wstring& key);
    void Set_Texture(const wstring& key);
    void  Set_Terrain(int CellSize, float scale);
    _float  Compute_Shade(int col, int row, _vec3 direct, int CellSpace);
    RENDER_PASS Get_RenderPass() override { return RENDER_PASS::RP_OPAQUE; };

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    LPDIRECT3DTEXTURE9 m_pTex;
    CTerrain* m_pMesh;
private:
    void Free() override;

    // CRenderer을(를) 통해 상속됨
};

END