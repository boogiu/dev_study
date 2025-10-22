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

    struct QueuedAnim
    {
        string Name;
        _float ConvertTime;
        _bool IsQueued = false;
        _uint animIndex = {};
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
    HRESULT Add_AnimClips(const string& LevelKey, const string& AnimKey, const string& Subject,_bool Loop = false);

public:
    virtual void Update_Animation(_float dt);

    const vector<_float4x4>& Get_BoneMatrices() { return m_FinalMatices; };
    const vector<_float4x4>& Get_CombinedBoneMatrices() { return m_CombinedMatrices; };
    virtual void Chane_Animation(_uint index, _float convertDuration = 0.2f);
    virtual HRESULT Chane_Animation(string animName, _float convertDuration = 0.2f);

    _bool isCurrentAnimEnd();
    string Get_CurrentAnimName();

public:
    void Control_Bone(const string& boneName, _fmatrix BoneMatrix);
    void Control_BoneByIndex(_uint Index, _fmatrix BoneMatrix);

public:
    _float4x4 Get_BoneMatrix(const string& boneName);
    _float4x4 Get_BoneMatrix(_uint Index);

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
    vector<_bool> m_pAnimLoops;
    unordered_map<string, _uint> m_pAnimNames;
    QueuedAnim m_QueuedAnim; /*다음 애니메이션 대기열*/

    _bool isAnimEnd = { false };
public:
    static CAnimator3D* Create();
    virtual CComponent* Clone();
    void Free() override;
};
NS_END
