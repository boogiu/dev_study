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
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;
    void Set_Mesh(const wstring& key);
    RENDER_PASS Get_RenderPass() override { return RENDER_PASS::RP_OPAQUE; };

public:
    CMesh* Get_Mesh() { return m_pMesh; };
    CMaterial* Get_Material() { return m_pMaterial; };

private:
    CMesh* m_pMesh;
    CMaterial* m_pMaterial;

private:
    void Free() override;
};

END