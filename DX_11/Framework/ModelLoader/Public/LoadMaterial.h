#pragma once
#include "Material.h"
NS_BEGIN(Loader)
class CLoadMaterial :
    public CMaterial
{
private:
    CLoadMaterial();
    CLoadMaterial(const CLoadMaterial& rhs);
    virtual ~CLoadMaterial() DEFAULT;
public:
    HRESULT Initialize();
    virtual void Render_GUI() override;
    HRESULT Load_Material(_uint materialNum, aiMaterial* material[], const string& filePath);
    HRESULT Save_Material ();
    void LinkShader(const string& shader);

private:
    vector<class CAIMaterial*> m_AIMaterialDatas;
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    string m_MaterialFileKey = {};

public:
    static CLoadMaterial* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END