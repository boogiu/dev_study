#pragma once
#include "Bone.h"
#include "Loader_Defines.h"
NS_BEGIN(Loader)
class CAIBone :
    public CBone
{
protected:
    CAIBone();
    virtual ~CAIBone() DEFAULT;
public:
    HRESULT Initialize(const aiNode* pAINode, _int iParentIndex);
    void Save_File(ofstream& ofs);
public:
    static CAIBone* Create(const aiNode* pAINode, _int iParentIndex);
    virtual void Free() override;
};

NS_END