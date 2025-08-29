#pragma once
#include "IService.h"

NS_BEGIN(Engine)

class ENGINE_DLL IResourceService : public IService
{
protected:
	~IResourceService() DEFAULT;
public:
    virtual HRESULT Initiallize()PURE;
    virtual FMOD::Sound* Load_Sound(const string& levelTag, const string& key)PURE;
    virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& key, INIT_DESC* pArg)PURE;
    virtual class CShader* Load_Shader(const string& levelTag, const string& key) PURE;

    virtual void Clear_Resource(const string& levelTag)PURE;
    virtual HRESULT Sync_To_Level()PURE;
    virtual void Load_InitialResource() PURE;

};

NS_END
