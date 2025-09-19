#pragma once
#include "Loader_Defines.h"
#include "MaterialData.h"
NS_BEGIN(Loader)
class CAIMaterial :
    public CMaterialData
{
private:
    CAIMaterial(const string& MaterialKey);
    virtual ~CAIMaterial() DEFAULT;
public:
    virtual HRESULT Initialize(ID3D11Device* pDevice, const aiMaterial* pAIMaterial,const string& fileDirectory);
public:
    void Save_MaterialData(ID3D11DeviceContext  * pContext,ofstream& ofs, const string& directory);
    void Render_GUI();
    void LinkShader(const string& shader);
private:
    int m_currentPassIndex = {};
public:
    static CAIMaterial* Create(ID3D11Device* pDevice, const aiMaterial* pAIMaterial, const string& MaterialKey, const string& fileDirectory);
    virtual void Free() override;
};

NS_END