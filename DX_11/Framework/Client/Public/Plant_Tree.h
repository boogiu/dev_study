#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CPlant_Tree :
    public CFieldObject
{
    enum Tree_State {
        IDLE,
        SHAKE, 
        SHAKING, 
        HITTED,
        CUTTED,
        ENCOUNTERED,
        DIGGED,
        STUMP
    };

private:
    CPlant_Tree();
    CPlant_Tree(const CPlant_Tree& rhs);
    virtual ~CPlant_Tree() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();
    virtual HRESULT Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;
    void Remove_Additional(const BaseEvent& event);

public:
    void EventListen(const BaseEvent& evt);
public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

private:
    void Normalize_Name(const string& modelName);
    void Add_Animation();
    void Check_State(_float dt);
    void Make_Fruits();
    void Adjust_Material();

private:
    void PlayAnim_Cut();
    void PlayAnim_Hit();
    void PlayAnim_Shake();
    void PlayAnim_Shaking();
    void PlayAnim_Encounter();

private:
    void Drop_Items();
    void Regenerate_Items();
    void Digged_Self(_float dt);

private:
    void Update_Bounce(_float dt);
    _float EaseInSigned(_float time);
    _float EaseIn(_float time);

private:
    _float2 LeafPalette = {};
    _float m_fWind = {};
    _float4x4 m_WindMat = {};
    
    TILE_INDEX m_Index = {};

    string m_ModelName = {};
    string m_TypeName = {};

    _bool m_isTargetRight = {};

    _bool m_isCutted = {false};
    _float m_fShakeTime = {};
    _uint m_iGrownLevel = {};
    _uint m_AxeHitCount = {};
    Tree_State m_eState = { IDLE };

    _float m_fLifeTime = {};

    _bool m_HasFruit = { false };
    class CItem_Object* m_pFruits[3] = { nullptr, nullptr, nullptr};

    string m_AdditionalData = {};

    _bool m_isEnded = { false };
     _float zRadian = {};
     _int BoneIndex = {4};

public:
    static CPlant_Tree* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END