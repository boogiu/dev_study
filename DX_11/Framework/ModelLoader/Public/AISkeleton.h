#pragma once
#include "Skeleton.h"
#include "Loader_Defines.h"

NS_BEGIN(Loader)
class CAISkeleton :
    public CSkeleton
{
private:
    CAISkeleton();
    virtual ~CAISkeleton() DEFAULT;
public:
    HRESULT Ready_Bones(const aiNode* pAINode, _int iParentIndex);

public:
    void Set_Offset(_uint Index, _float4x4 offset);
    HRESULT Save_Skeletons(const string& path);
    void Save_File(ofstream& ofs);

public:
    _int Find_SimilarByName(const string& meshName);
public:
    static CAISkeleton* Create(const aiNode* pAINode);
    virtual void Free() override;
};
NS_END
