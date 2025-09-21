#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CMaterial :
    public CComponent
{
protected:
   
    CMaterial();
    CMaterial(const CMaterial& rhs);
    virtual ~CMaterial() DEFAULT;
public:
   virtual HRESULT Initialize_Prototype() override;
   virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;
public:
    HRESULT Link_Material(const string& levelKey, const string& materialKey);
    class CShader* Get_Shader(_uint Index);
    _uint Get_ShaderID(_uint Index);
    _uint Get_MaterialDataID(_uint Index);
    void Apply_Material(ID3D11DeviceContext* pContext, _uint Index);
    HRESULT GetPassSignature(_uint Index, D3DX11_PASS_DESC* pOutPassDesc);
    const string& GetPassConstant(_uint Index);
public:
    virtual void Render_GUI() override;
protected:
    vector<class CMaterialData*> m_MaterialDatas;
    _uint m_TextureIndex = {};
    //map<TEXTURE_TYPE, _uint> -> 각 타입별 현재 프레임 인덱스 관리
#ifdef _DEBUG
    _bool m_bMaterialTabOpen = {false};
#endif // _DEBUG

public:
    static CMaterial* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};
NS_END
