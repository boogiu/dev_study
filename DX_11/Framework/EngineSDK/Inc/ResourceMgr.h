#pragma once
#include "IResourceService.h"

NS_BEGIN(Engine)

class CResourceMgr final :
	public IResourceService
{
	struct RS_Pool {
		unordered_map<string, class CShader*>m_Shaders;
		unordered_map<string, class CVIBuffer*>m_Buffers;
		unordered_map<string, class CModelData*>m_ModelDatas;
		unordered_map<string, class vector<class CMaterialData*>>m_MaterialDatas;
		unordered_map<string, class CTexture*>m_Textures;
		unordered_map<string, class CSoundData*>m_Sounds;
	};

private:
	CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CResourceMgr() DEFAULT;

public:
	HRESULT Initiallize();
	virtual void Clear_Resource(const string& levelTag)override;
	virtual HRESULT Sync_To_Level()override;

public:
	virtual class CSoundData* Load_Sound(const string& levelTag, const string& soundKey) override;
	virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& bufferKey, BUFFER_TYPE eType) override;
	virtual const vector<class CMaterialData*>& Load_MaterialData(const string& levelTag, const string& materialKey) override;
	virtual class CShader* Load_Shader(const string& levelTag, const string& shaderKey) override;
	virtual class CTexture* Load_Texture(const string& levelTag, const string& textureKey) override;
	virtual class CModelData* Load_ModelData(const string& levelTag, const string& ModelKey) override;

	virtual string Get_ResourcePath(const string& resourceKey) override;
	virtual HRESULT Add_ResourcePath(const string& resourceKey, const string& resourcePath) override;

private:
	_int ValidLevel(const string& levelKey);
	void Load_InitialResource();

private:
	string MakePath(const string& pathKey);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class CGameInstance* m_pInstance = { nullptr };

	unordered_map<string, _uint> m_LevelIndex;
	vector<RS_Pool >m_Resources;
	unordered_map<string, string>m_KeyPath;
public:
	static CResourceMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};


NS_END

