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
    virtual HRESULT Link_Buffer(const string& levelKey, const string& MeshKey);
   virtual  _uint Get_ElementCount()PURE;
   virtual const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() PURE;
   virtual HRESULT Render_Mesh(ID3D11DeviceContext* pContext,_uint Index) PURE;
  
public:
     _uint Get_MaterialIndex(_uint Index);
    _bool hasBuffer() { return !m_Buffers.empty(); }
    _bool isDrawable(_uint Index);
    const string& Get_BufferKey(_uint Index);
    _uint Get_MeshCount() { return m_Buffers.size(); }

public:
    virtual void Render_GUI() override;

protected:
    vector<_bool> m_DrawableMeshes;
    vector<class CMesh*> m_Buffers;

public:
    virtual void Free() override;
};

NS_END