#pragma once
#include "Base.h"
NS_BEGIN(Engine)

class ENGINE_DLL CShader final:
    public CBase
{
private:
    CShader();
    virtual ~CShader() override;

public:
    HRESULT Initialize(ID3D11Device* pDevice,  const string& filePath);
    HRESULT GetPassSignature(UINT iPassIndex, D3DX11_PASS_DESC* pOutPassDesc);
    virtual const string& Get_Key() { return m_ShaderKey; }
    virtual void Set_Key(const string& key) { m_ShaderKey = key; }

private:
    ID3DX11Effect* m_pEffect = { nullptr };
    ID3DX11EffectTechnique* m_pTechnique = { nullptr };
    string m_ShaderKey;

public:
   static CShader* Create(ID3D11Device* pDevice, const string& filePath);
   virtual void Free() override;
};

NS_END