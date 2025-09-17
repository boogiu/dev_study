#pragma once
#include "Bone.h"
#include "Loader_Defines.h"
NS_BEGIN(Loader)
class CLoadBone :
    public CBone
{
protected:
    CLoadBone();
    virtual ~CLoadBone() DEFAULT;
public:
    HRESULT Initialize(const aiNode* pAINode, _int iParentIndex);
public:
    static CLoadBone* Create(const aiNode* pAINode, _int iParentIndex);
    virtual void Free() override;
};

NS_END