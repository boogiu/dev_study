#pragma once
#include "CRenderer.h"

BEGIN(Engine)
class CMesh;
class CGameObject;
class CMaterial;

class ENGINE_DLL CReflectRenderer :
    public CRenderer
{
private:
    explicit CReflectRenderer();
    virtual ~CReflectRenderer();
public:
    static CReflectRenderer* Create();
private:
    virtual HRESULT Ready_Component() override;

public:
    void Update_Component(float& dt) override;
    void LateUpdate_Component(float& dt) override;
    virtual void Render(LPDIRECT3DDEVICE9 pDevice) override;
    virtual CComponent* Clone() const override;
    void Set_Mesh(const wstring& key, vector<DWORD> subsetIdx);

    RENDER_PASS Get_RenderPass() override { return RENDER_PASS::RP_STENCIL; };
private:
    void Compute_Plane();
    void Get_CandidateObject();
    void Set_TargetObject();

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    CMesh* m_pMesh;
    CMaterial* m_pMaterial;

    vector<DWORD> m_SubsetIdx;
    D3DXPLANE m_tPLane;

    vector<CGameObject*> m_Candidate; //후보군-> 나중에 오브젝트 매니저에서 가져올 것.
    vector<CGameObject*> m_Targets; //실제 타겟들 걸러서 넣어짐
private:
    virtual void Free();
};

END