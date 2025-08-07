#pragma once
#include "CBase.h"

BEGIN(Engine)
class CScene;

class ENGINE_DLL CSceneMgr :
    public CBase
{
    DECLARE_SINGLETON(CSceneMgr)

private:
    explicit CSceneMgr();
    virtual ~CSceneMgr();

public:
    HRESULT Ready_Scene();
    void Update_Scene(_float& dt);
    void LateUpdate_Scene(_float& dt);
    void Render_Scene(LPDIRECT3DDEVICE9 pDevice);

public:
    void Change_Scene(const wstring& key);
    CScene* Find_Scene(const wstring& key);
    HRESULT Add_Scene(const wstring& key, CScene* scene);
    CScene* Get_NowScene() { return m_curScene; }

private:
    CScene* m_curScene;
    unordered_map<wstring, CScene*> m_SceneContainer;

private:
    void Free();
};

END
