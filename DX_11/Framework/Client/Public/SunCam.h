#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CSunCam :
    public CGameObject
{
private:
    CSunCam();
    CSunCam(const CSunCam& rhs);
    virtual ~CSunCam()DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Render_GUI() override;

public:
    void Set_Target(CGameObject* pObj) { m_pTarget = pObj; };
    void Set_Night();

private:
    _float m_InitialHeight = { 120 };
    CGameObject* m_pTarget = { nullptr };

public:
    static CSunCam* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free() override;
};
NS_END
