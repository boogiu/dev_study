#pragma once
#include "IResourceService.h"
#include "ResourcePool.h"

NS_BEGIN(Engine)
class CResourceMgr final:
    public IResourceService
{
private:
    CResourceMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    ~CResourceMgr();

public:
    virtual HRESULT Initiallize();
    virtual FMOD::Sound* Load_Sound(const string& levelTag, const string& key);
    virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& key);

    virtual void Clear_Resource(const string& levelTag);
    virtual HRESULT Sync_To_Level();
    virtual void Load_InitialResource();

private:
    ID3D11Device* m_pDevice = {nullptr};
    ID3D11DeviceContext* m_pContext = { nullptr };
    unordered_map<string, ResourcePool<class CVIBuffer>> m_Buffers;
    unordered_map<string, ResourcePool<class CShader>> m_Shaders;
    unordered_map<string, ResourcePool<FMOD::Sound>> m_Sounds;

public:
    static CResourceMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override; 
};
NS_END
 