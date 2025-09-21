#pragma once
#include "Base.h"
#include "Shader.h"

NS_BEGIN(Engine)
using TEXTUERS = vector<class CTexture*>;

class ENGINE_DLL CMaterialData :
	public CBase
{
protected:
	CMaterialData(const string& MaterialKey);
	virtual ~CMaterialData();
public:
	HRESULT Initialize(ID3D11Device* pDevice, const string& levelKey, ifstream& ifs, const string& directory);
	HRESULT CreateCBuffer(ID3D11Device* pDevice);

public:
	void ApplyData(ID3D11DeviceContext* pContext, _uint Index);
	HRESULT GetPassSignature(D3DX11_PASS_DESC* pOutPassDesc);

public:
	_uint  Get_ShaderID() { return m_pShader->Get_ID(); }
	_uint  Get_MaterialDataID() { return m_DataID; }
	class CShader* Get_Shader() { return m_pShader; }
	const MaterialConstants& Get_MaterialConstant() { return m_MaterialConstant; }
	const string&  Get_PassConstant() { return m_passConstant; }

public:
	virtual void Render_GUI();

protected:
	virtual HRESULT Link_Texture(const string& levelKey, const string& textureKey, TEXTURE_TYPE eType);
	virtual HRESULT Link_Shader(const string& levelKey, const string& shaderKey);
	string ConvertToConstant(TEXTURE_TYPE eType);

protected:
	_uint m_DataID = {};
	string m_MaterialKey = {};
	string m_passConstant = {};
	class CShader* m_pShader = { nullptr };
	ID3D11Buffer* m_pMaterialCBuffer = { nullptr };
	MaterialConstants m_MaterialConstant = {};
	map<TEXTURE_TYPE, TEXTUERS> m_Textures;
public:
	static CMaterialData* Create(ID3D11Device* pDevice, const string& levelKey,const string& MaterialKey, ifstream& ifs, const string& directory);
	virtual void Free() override;
	static _uint s_NextID;
};

NS_END