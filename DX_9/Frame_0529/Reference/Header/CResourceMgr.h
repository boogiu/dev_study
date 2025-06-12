#pragma once
#include "CBase.h"
BEGIN(Engine)

class IMesh;
class CMaterial;

class ENGINE_DLL CResourceMgr :
    public CBase
{
    DECLARE_SINGLETON(CResourceMgr)

private:
    explicit CResourceMgr();
    virtual ~CResourceMgr();

public: 
    HRESULT Ready_Resoource();

    IMesh* Find_Mesh(const string& key);
    CMaterial* Find_Material(const string& key);
    const LPDIRECT3DTEXTURE9& Find_Texture(const string& key);

    HRESULT Load_All();
    void LoadStaticMeshByX(const string& key);
    void LoadTerrainMeshByRaw(const string& key, int x, int z);
    void LoadAllTexture();

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    //동적/정적 메쉬들 컨테이너
    unordered_map<string, IMesh*> m_MeshContainer;
    //머티리얼 컨테이너
    unordered_map<string, CMaterial*> m_MatContainer;
    //텍스쳐 컨테이너
    unordered_map< string,LPDIRECT3DTEXTURE9> m_TexContainer;

private:
    void Free();
};

END