#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFishSub_Tool :
    public CGameObject
{
private:
    CFishSub_Tool();
    CFishSub_Tool(const CFishSub_Tool& rhs);
    virtual ~CFishSub_Tool() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context) override;
    void OnCollisionExit(COLLISION_CONTEXT context) override;

public:
    static CFishSub_Tool* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
