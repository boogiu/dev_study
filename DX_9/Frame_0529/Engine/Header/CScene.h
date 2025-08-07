#pragma once
#include "CBase.h"

BEGIN(Engine) 

class CLayer;
class CGameObject;
class CStateCache;

class ENGINE_DLL CScene :
    public CBase
{
protected:
    explicit CScene();
    virtual ~CScene();

public :
    virtual HRESULT Ready_Scene()PURE;
    virtual HRESULT Enter_Scene()PURE;
    virtual HRESULT Exit_Scene()PURE;

    virtual void Update_Scene(_float& dt)PURE;
    virtual void LateUpdate_Scene(_float& dt) PURE;
    virtual void Render_Scene(LPDIRECT3DDEVICE9 pDevice) PURE;

protected:
    void Create_Layer(const wstring& key);
    void Free_Layer(const wstring& key);
    void Free_AllLayer();

protected:
    CStateCache* m_pCache;
    unordered_map<wstring, CLayer*> m_mapLayer;
};

END