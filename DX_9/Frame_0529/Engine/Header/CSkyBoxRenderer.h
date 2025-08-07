#pragma once
#include "CRenderer.h"
BEGIN(Engine)
class IMesh;

class  ENGINE_DLL CSkyBoxRenderer :
    public CRenderer
{
private:
    explicit CSkyBoxRenderer();
    virtual ~CSkyBoxRenderer();
public:
    static CSkyBoxRenderer* Create();
public:
    HRESULT Ready_Component() override;
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;

    void Set_SkyBox(const wstring& _key);
    RENDER_PASS Get_RenderPass() override { return RENDER_PASS::RP_SKYBOX; };

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    LPDIRECT3DCUBETEXTURE9 m_cubeTexture;
    IMesh*  m_pCube;

private:
    void Free() override;
};
END
