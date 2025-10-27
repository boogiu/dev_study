#pragma once
#include "FieldObject.h"
NS_BEGIN(Client)
class CPlant_Tree :
    public CFieldObject
{
    enum Tree_State {IDLE, SHAKE, CUTTED};

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
    void ConvertModel();

private:
    _bool isReadyToAnimate = { false };

    string m_ModelName = {};
    string m_BaseName = {};
    string m_NodeName = {};
    string m_StumpName = {};

    _uint m_AxeHitCount = {};
    Tree_State m_eState = { IDLE };
    TILE_INDEX m_Index = {};
public:
    static CPlant_Tree* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END