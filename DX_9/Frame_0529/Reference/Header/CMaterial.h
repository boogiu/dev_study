#pragma once
#include "CBase.h"
BEGIN(Engine)

class ENGINE_DLL CMaterial :
    public CBase
{
private:
    explicit CMaterial();
    virtual ~CMaterial();

public:
    static CMaterial* Create();

public:
    HRESULT Ready_Material();

public:
    void SetSize(int i);
    void SetKey(const string& key) { m_key = key; };
    vector<MATTEX>& Get_Material() { return m_vecMaterial; };

private:
    string m_key;
    vector<MATTEX> m_vecMaterial;

private:
    void Free();
};

END