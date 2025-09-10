#pragma once
#include "IService.h"

NS_BEGIN(Engine)

class ENGINE_DLL IResourceService : public IService
{
protected:
	~IResourceService() DEFAULT;
public:
    virtual HRESULT Initiallize()PURE;

    virtual class CSoundData* Load_Sound(const string& levelTag, const string& key) PURE;
    virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& key, BUFFER_TYPE m_eType) PURE;

    virtual class CShader* Load_Shader(const string& levelTag, const string& key) PURE;
    virtual class CTexture* Load_Texture(const string& levelTag, const string& key) PURE;

    virtual HRESULT Add_ResourcePath(const string& levelTag, const string& key, const string& path) PURE;

    virtual void Clear_Resource(const string& levelTag)PURE;
    virtual HRESULT Sync_To_Level()PURE;
    virtual void Load_InitialResource() PURE;

};

NS_END
