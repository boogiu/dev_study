#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CFurniture :
    public CGameObject
{
protected:
    CFurniture();
    CFurniture(const CFurniture& rhs);
    virtual ~CFurniture() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(INIT_DESC* pArg) override;
    virtual void Priority_Update(_float dt) override;
    virtual void Update(_float dt) override;
    virtual void Late_Update(_float dt) override;

public:
    virtual virtual void Render_GUI()override;

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context)override;
    void OnCollisionExit(COLLISION_CONTEXT context)override;

public:
    HRESULT Make_FurnitureByMapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable);
    void EventListen(const BaseEvent& evt);
private:
    void Update_Bounce(_float dt);
    void Update_LightBounce(_float dt);
    _float EaseIn(_float time);
protected:
    string m_FurnitureName = {};
    TILE_INDEX m_Index = {};
    _bool m_bLightObject = {};
    _bool m_bAmpObject = {};
    _float m_ElapsedTime = {};

    _bool m_bEmitEffect= {};
public:
    static CFurniture* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END