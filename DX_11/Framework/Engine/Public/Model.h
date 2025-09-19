#pragma once
#include "Component.h"
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel :
    public CComponent
{
protected:
    CModel();
    CModel(const CModel& rhs);
    virtual ~CModel() DEFAULT;

public:
    virtual HRESULT Link_Model(const string& levelKey, const string& MeshKey) PURE;
    virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc(_uint DrawIndex);
    virtual const _uint Get_ElementCount(_uint DrawIndex);
    virtual const string_view Get_ElementKey(_uint DrawIndex);
   virtual HRESULT Render_Model(ID3D11DeviceContext* pContext,_uint Index) PURE;
  
public:
    _uint Get_MeshCount();
    _uint Get_MaterialIndex(_uint Index);
    _bool isDrawable(_uint Index);

public:
    virtual void Render_GUI() override;

protected:
    vector<_bool> m_DrawableMeshes;
    class CModelData* m_pData = { nullptr };
public:
    virtual void Free() override;
};

NS_END