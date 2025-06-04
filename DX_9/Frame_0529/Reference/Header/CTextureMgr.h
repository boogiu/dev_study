#pragma once
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CTextureMgr :
    public CBase
{
    DECLARE_SINGLETON(CTextureMgr)

private:
    explicit CTextureMgr();
    virtual ~CTextureMgr();

public:
    HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pDevice, const string folderPath);
    LPDIRECT3DTEXTURE9 Get_Texture(const string& key);
private:
    unordered_map<string, LPDIRECT3DTEXTURE9> m_textureContainer;

private:
    void Free() override;
};

END