#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CSkyBox :
    public CGameObject
{
private:
    CSkyBox();
    CSkyBox(const CSkyBox& rhs);
    virtual ~CSkyBox() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;

private:
    _float m_ElapsedTime = {};
    _float4 m_SkyColor = {1.f,1.f,1.f,1.f};
public:
    static CSkyBox* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
