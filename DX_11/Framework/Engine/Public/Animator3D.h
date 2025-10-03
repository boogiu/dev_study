#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CAnimator3D :
    public CComponent
{
protected:
    enum class ANIMATOR_STATE{RUNNING, CONVERTING};
    struct AnimConvert {
        _uint SrcClip;
        _uint DstClip;
        _float ConversionElapsedTime = {};
        _float ConversionDuration = {};
    };

protected:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& rhs);
    ~CAnimator3D() DEFAULT;


public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    void LinkAnimate_Model(const string& LevelKey, const string& ModelKey);
    virtual void Update_Animation(_float dt);
    const vector<_float4x4>& Get_BoneMatrices() { return m_FinalMatices; };
    virtual void Chane_Animation(_uint index, _float convertDuration = 0.2f);

public:
    void Control_Bone(const string& boneName, _fmatrix BoneMatrix);
    void Control_BoneByIndex(_uint Index, _fmatrix BoneMatrix);

protected:
    void Animation_Run(_float dt);
    void Animation_Convert(_float dt);
    void BuildBone();

public:
    virtual void Render_GUI();

protected:
    class CModelData* m_pData = {};
    ANIMATOR_STATE m_eState = {};

    _uint m_iCurrentClipIndex = {};
    _uint m_iNextClipIndex = {}; //다음 애니메이션 전환 용
    _float m_fConvertDuration = {}; //다음 애니메이션 전환 용
    _float m_fPrevTrackPosition = {}; //다음 애니메이션 전환 용

    _float m_fCurrentTrackPosition = {};

    vector<_float4x4> m_TransfromationMatrices = {};
    vector<_float4x4> m_ManipulateMatrices = {};
    vector<_float4x4> m_CombinedMatrices = {};
    vector<_float4x4> m_FinalMatices = {};
    vector<class CAnimationClip*> m_pAnimClips;

public:
    static CAnimator3D* Create();
    virtual CComponent* Clone();
    void Free() override;
};
NS_END
