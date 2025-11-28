#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CCloud_Env :
    public CGameObject
{
private:
    CCloud_Env();
    CCloud_Env(const CCloud_Env& rhs);
    virtual ~CCloud_Env() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
public:
    virtual void Render_GUI()override;
public:
    void Render_Cloud(ID3D11DeviceContext* pContext);
    void Render_CloudChild(ID3D11DeviceContext* pContext);
private:
    _float4x4 m_ViewMatrix = {};
    _float4x4 m_ProjMatrix = {};

    vector<class CCloud_Space*> m_Clouds;

public:
    static CCloud_Env* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END
