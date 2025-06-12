#pragma once
#include "CRenderer.h"
BEGIN(Engine)
class CCubeMesh;

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
    void Set_TestCubeTexture();
private:
    HRESULT Set_Buffer();
private:
    LPDIRECT3DDEVICE9 m_pDevice;
    LPDIRECT3DCUBETEXTURE9  m_pTexture;
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    LPDIRECT3DINDEXBUFFER9 m_pIB;
private:
    void Free() override;
};
END
