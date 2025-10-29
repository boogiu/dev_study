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
        ENCOUNTERED
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
    virtual HRESULT Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;

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

private:
    TILE_INDEX m_Index = {};
    string m_ModelName = {};
    string m_TypeName = {};
    _uint m_iGrownLevel = {};
    _float m_fShakeTime = {};
    _bool m_isTargetRight = {};

    _uint m_AxeHitCount = {};
    Tree_State m_eState = { IDLE };

    class CPlant_Fruit* m_pFruits[3];

    _bool m_isAbleToDrop = { true };
    _float2 LeafPalette = {};
public:
    static CPlant_Tree* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END