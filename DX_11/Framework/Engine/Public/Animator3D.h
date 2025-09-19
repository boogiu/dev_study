#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CAnimator3D :
    public CComponent
{
protected:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& rhs);
    ~CAnimator3D() DEFAULT;


public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    //void LinkAnimate_Model(const string& ModelKey);
    virtual void Update_Animation(_float dt);
    const vector<_float4x4>& Get_BoneMatrices() { return m_FinalMatices; };
    virtual void Chane_Animation(_uint index);
public:
    virtual void Render_GUI();

protected:
    class CModelData* m_pData = {};

    _uint m_iCurrentClipIndex = {};
    _float m_fCurrentTrackPosition = {};

    vector<_float4x4> m_TransfromationMatrices = {};
    vector<_float4x4> m_CombinedMatrices = {};
    vector<_float4x4> m_FinalMatices = {};
    vector<class CAnimationClip*> m_pAnimClips;

public:
    static CAnimator3D* Create();
    virtual CComponent* Clone();
    void Free() override;
};
NS_END
