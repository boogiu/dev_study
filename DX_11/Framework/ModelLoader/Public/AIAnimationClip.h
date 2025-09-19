#pragma once
#include "AnimationClip.h"
NS_BEGIN(Engine)
class CModelData;
NS_END
NS_BEGIN(Loader)
class CAIAnimationClip :
    public CAnimationClip
{
private:
    CAIAnimationClip();
   ~ CAIAnimationClip() DEFAULT;
public:
    HRESULT Initialize(const aiAnimation* pAIAnimation, class CModelData* pData);
public:
    virtual void Render_GUI() override;
    void Change_Loop() { m_bLoop = !m_bLoop; }
    HRESULT Save_AnimationClip(const string& DirectoryPath);
    void Set_ClipName(const string& fileName) { m_ClipName = fileName; };
public:
    static CAIAnimationClip* Create(const aiAnimation* pAIAnimation,class CModelData* pData);
    virtual void Free() override;
};

NS_END