#pragma once
#include "Skeleton.h"
#include "Loader_Defines.h"

NS_BEGIN(Loader)
class CLoadedSkeleton :
    public CSkeleton
{
private:
    CLoadedSkeleton();
    virtual ~CLoadedSkeleton() DEFAULT;
public:
    HRESULT Ready_Bones(const aiNode* pAINode, _int iParentIndex);
    HRESULT Create_Bone(const string& MeshName);

public:
    static CLoadedSkeleton* Create(const aiNode* pAINode);
    virtual void Free() override;
};
NS_END
