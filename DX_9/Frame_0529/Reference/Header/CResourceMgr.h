#pragma once
#include "CBase.h"
BEGIN(Engine)

class IMesh;
class CMaterial;
class CFont;

class ENGINE_DLL CResourceMgr :
    public CBase
{
    DECLARE_SINGLETON(CResourceMgr)

private:
    explicit CResourceMgr();
    virtual ~CResourceMgr();

public: 
    HRESULT Ready_Resoource();

    IMesh* Find_Mesh(const wstring& key);
    CMaterial* Find_Material(const wstring& key);
     CFont* Find_Font(const wstring& key);
    const LPDIRECT3DTEXTURE9& Find_Texture(const wstring& key);

    HRESULT Load_All();
    void LoadDeafultMesh();
    void LoadStaticMeshByX(const wstring& key);
    void LoadTerrainMeshByRaw(const wstring& key, int x, int z);
    void LoadTerrainMeshByBMP(const wstring& key, int x, int z);
    void LoadAllTexture();
    void LoadFonts(const wstring& key);

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    //동적/정적 메쉬들 컨테이너
    unordered_map<wstring, IMesh*> m_MeshContainer;
    //머티리얼 컨테이너
    unordered_map<wstring, CMaterial*> m_MatContainer;
    //텍스쳐 컨테이너
    unordered_map< wstring,LPDIRECT3DTEXTURE9> m_TexContainer;
    //폰트 컨테이너
    unordered_map< wstring, CFont*> m_FontContainer;

private:
    void Free();
};

END