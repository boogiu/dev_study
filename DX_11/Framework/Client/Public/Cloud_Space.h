#pragma once
#include "UI_Object.h"

NS_BEGIN(Engine)
class CMaterialInstance;
NS_END
NS_BEGIN(Client)
class CCloud_Space :
    public CGameObject
{
private:
    CCloud_Space();
    CCloud_Space(const CCloud_Space& rhs);
    virtual ~CCloud_Space() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI() override;

public:
    void Render(ID3D11DeviceContext* pContext) ;
    void Set_Matrix(_float4x4* view, _float4x4* proj) { m_matViewEnvShadow = view; m_matProjEnvShadow = proj; }
private:
    _float m_ElapsedTime = {};
    _float m_ZTime = {};
    _float m_BaseSpeed = {};
    _float  m_TimeOffset = 0.f;   // 시작 위상 랜덤
    _float3 m_MoveScale = { 1.f, 1.f, 1.f }; // 축별 스케일 랜덤

     _float4x4* m_matViewEnvShadow = { nullptr };
     _float4x4* m_matProjEnvShadow = { nullptr };
    class CMaterialInstance* customInstance = { nullptr };

public:
    static CCloud_Space* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END