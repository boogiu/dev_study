#pragma once
#include "CRenderer.h"

BEGIN(Engine)
class CMesh;
class CMaterial;
class CTexture;

class ENGINE_DLL CMeshRenderer :
    public CRenderer
{
private:
    explicit CMeshRenderer();
    virtual ~CMeshRenderer() override;

public:
    static CMeshRenderer* Create();

public:
    HRESULT Ready_Component() override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;
    void Set_Mesh(const string& key);

private:
    CMesh* m_pMesh;
    CMaterial* m_pMaterial;

private:
    void Free() override;
};

END