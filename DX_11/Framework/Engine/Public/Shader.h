#pragma once
#include "Base.h"
NS_BEGIN(Engine)
typedef struct Shader_Desc : public INIT_DESC {
    string ShaderPath = {};
    D3D11_INPUT_ELEMENT_DESC* pElements;
    _uint iElementCount;
};

class ENGINE_DLL CShader final:
    public CBase
{
private:
    CShader();
    virtual ~CShader() override;

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Shader_Desc* pArg);

private:
    ID3DX11Effect* m_pEffect = { nullptr };
    vector<ID3D11InputLayout*> m_InputLayouts;
    _uint m_iPassCount = {};
public:
   static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Shader_Desc*pArg);
   virtual void Free() override;
};

NS_END