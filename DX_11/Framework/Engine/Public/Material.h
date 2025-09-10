#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CMaterial :
    public CComponent
{
    struct MaterialConstants
    {
        _float4 vMtrDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
        _float4 vMtrlAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
        _float4 vMtrlSpecular = _float4(1.0f, 1.0f, 1.0f, 1.f);
        _float fSpecularPow = { 0.1f };
        _float3 vPadding; 
       //_float fShininess = 32.f;
    };
private:
    CMaterial();
    CMaterial(const CMaterial& rhs);
    virtual ~CMaterial() DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;
    HRESULT Link_Shader(const string& levelKey, const string& key);
    HRESULT Link_Texture(const string& levelKey, const string& key, const string& constant);
public:
    void Apply_Material(const string& passConstant, ID3D11DeviceContext* pContext);
    class CShader* Get_Shader() { return m_pShader; }
public:
    virtual void Render_GUI() override;
private:
    void Bind_InternalValues(ID3D11DeviceContext* pContext);
private:
    class CShader* m_pShader = { nullptr };
    unordered_map<string, class CTexture*> m_Textures;
    MaterialConstants m_MaterialConstant = {};
    ID3D11Buffer* m_pMaterialCBuffer = {nullptr};

public:
    static CMaterial* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};
NS_END
