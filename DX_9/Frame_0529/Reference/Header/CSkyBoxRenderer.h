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
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    CComponent* Clone() const override;
    void Set_Texture(const string& _key);
private:
    HRESULT Set_Buffer();
private:
    LPDIRECT3DDEVICE9 m_pDevice;
    LPDIRECT3DTEXTURE9 m_pTexture;
    IMesh* m_pMesh;
private:
    void Free() override;
};
END
