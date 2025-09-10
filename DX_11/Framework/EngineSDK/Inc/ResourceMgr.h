#pragma once
#include "IResourceService.h"
#include "ResourcePool.h"

NS_BEGIN(Engine)
class CResourceMgr final :
	public IResourceService
{
private:
	CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CResourceMgr();

public:
	HRESULT Initiallize();
	virtual class CSoundData* Load_Sound(const string& levelTag, const string& key) override;
	virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& key, BUFFER_TYPE m_eType) override;
	virtual class CShader* Load_Shader(const string& levelTag, const string& key) override;
	virtual class CTexture* Load_Texture(const string& levelTag, const string& key) override;

	HRESULT Add_ResourcePath(const string& levelTag, const string& key, const string& path) override;

	virtual void Clear_Resource(const string& levelTag)override;
	virtual HRESULT Sync_To_Level()override;
	virtual void Load_InitialResource()override;

private:
	string MakePath(const string& levelTag, const string& key);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class CGameInstance* m_pInstance = { nullptr };
	unordered_map<string, ResourcePool<class CVIBuffer>> m_Buffers;
	unordered_map<string, ResourcePool<class CShader>> m_Shaders;
	unordered_map<string, ResourcePool<class CTexture>> m_Textures;
	unordered_map<string, ResourcePool<class CSoundData>> m_Sounds;

	unordered_map<string, ResourcePool<string>> m_filePath;
public:
	static CResourceMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};
NS_END
