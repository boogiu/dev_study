#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CSprite2D final:
    public CComponent
{
private:
    CSprite2D();
    CSprite2D(const CSprite2D& rhs);
    virtual ~CSprite2D();
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    /*텍스처는 부모 폴더와 함께 ParentName + TextureKey*/
    HRESULT Add_Texture(const string& levelKey, const string& TextureKey);
    HRESULT Link_Shader(const string& levelKey, const string& shaderKey);

public:
    void Apply_Shader(ID3D11DeviceContext* pContext);
    void Draw_Sprite(ID3D11DeviceContext* pContext);
public:
    class CShader* Get_Shader() {return m_pShader;};
    class  CVIBuffer* Get_Buffer();
    const string& Get_PassConstant() { return m_PassConstant; }

public:
    _bool IsValid();
public:
    void Render_GUI();

private:
    _uint m_iDrawIndex = {};
    string m_PassConstant = {"Opaque"};
    class  CVI_Point* m_pPoint = {nullptr};
    class  CShader* m_pShader = { nullptr };
    vector<class CTexture*> m_pTextures;
public:
    static CSprite2D* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};
NS_END

