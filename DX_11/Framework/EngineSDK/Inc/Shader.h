#pragma once
#include "Base.h"
NS_BEGIN(Engine)

class ENGINE_DLL CShader final:
    public CBase
{
public:
    typedef struct ShaderVariableTypeDesc {
        string   parentCBufferName = {};
        string typeName = {};
        string constantName = {};
        _uint varSize = {};
        ID3DX11EffectVariable* pHandle = {nullptr};
    }SHADER_VAR_DESC;

    typedef struct ShaderConstantBufferDesc {
        string Name;
        uint32_t RegisterSlot;
        uint32_t Size;
        ID3DX11EffectConstantBuffer* pHandle = { nullptr };
    } CBUFFER_DESC;
private:
    CShader();
    virtual ~CShader() override;

public:
    HRESULT Initialize(ID3D11Device* pDevice,  const string& filePath);
    HRESULT GetPassSignature(UINT iPassIndex, D3DX11_PASS_DESC* pOutPassDesc);
    HRESULT GetPassSignature(const string& passConstant, D3DX11_PASS_DESC* pOutPassDesc);
    const string& Get_Key() { return m_ShaderKey; }
    void Set_Key(const string& key) { m_ShaderKey = key; }
    void Apply(const string& passConstant,ID3D11DeviceContext* pContext);

    HRESULT Bind_Value(const string& ConstantName, void* pData, _uint size);
    HRESULT SetConstantBuffer(const string& ConstantName, ID3D11Buffer* pData);

private:
    HRESULT Bind_Matrix(const string& ConstantName, const _float4x4* pMatrix);
    HRESULT Bind_ShaderResource(const string& ConstantName, ID3D11ShaderResourceView* pSRV);
    void ReflectShader();

private:
    string m_ShaderKey;
    wstring m_FileName = {};

    ID3DX11Effect* m_pEffect = { nullptr };
    ID3DX11EffectTechnique* m_pTechnique = { nullptr };

    unordered_map<string, ID3DX11EffectPass*> m_Passes;         /*패스들 미리 저장*/
    unordered_map<string, SHADER_VAR_DESC> m_Variables;     /*변수들 미리 저장*/
    unordered_map<string, CBUFFER_DESC> m_CBuffers;             /*상수버퍼 미리 저장*/

public:
   static CShader* Create(ID3D11Device* pDevice, const string& filePath, const string& key);
   virtual void Free() override;
};

NS_END