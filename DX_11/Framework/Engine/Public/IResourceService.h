#pragma once
#include "IService.h"

NS_BEGIN(Engine)

class ENGINE_DLL IResourceService : public IService
{
protected:
	~IResourceService() DEFAULT;
public:
    virtual HRESULT Initiallize()PURE;

    virtual class CSoundData* Load_Sound(const string& levelTag, const string& imguiID) PURE;
    virtual class CVIBuffer* Load_VIBuffer(const string& levelTag, const string& imguiID, BUFFER_TYPE eType) PURE;
    virtual const vector<class CMesh*>& Load_Mesh(const string& levelTag, const string& imguiID) PURE;
    virtual const vector<class CMaterialData*>& Load_MaterialData(const string& levelTag, const string& materialKey) PURE;
    virtual class CTexture* Load_Texture(const string& levelTag, const string& imguiID) PURE;
    virtual class CShader* Load_Shader(const string& levelTag, const string& imguiID) PURE;
    virtual class CSkeleton* Load_Skeleton(const string& levelTag, const string& skeletonKey) PURE;

    virtual string Get_ResourcePath(const string& resourceKey) PURE;
    virtual HRESULT Add_ResourcePath(const string& resourceKey, const string& resourcePath) PURE;
    virtual void Clear_Resource(const string& levelTag)PURE;
    virtual HRESULT Sync_To_Level()PURE;
};

NS_END
