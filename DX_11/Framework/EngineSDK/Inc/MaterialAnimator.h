#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CMaterialAnimator :
    public CComponent
{
    typedef struct MaterialAnimationClip{

    }MA_CLIP;

private:
    CMaterialAnimator();
    CMaterialAnimator(const CMaterialAnimator& rhs);
    virtual ~CMaterialAnimator() DEFAULT;
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    static CMaterialAnimator* Create();
    virtual CComponent* Clone();
    void Free() override;
};

NS_END