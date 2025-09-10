#pragma once
#include "Component.h"
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel :
    public CComponent
{
private:
    CModel();
    CModel(const CModel& rhs);
    virtual ~CModel();
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(COMPONENT_DESC* pArg) override;
public:
    HRESULT Link_Buffer(const string& levelKey, const string& bufferKey, BUFFER_TYPE eType);
    void Link_Element(const D3D11_INPUT_ELEMENT_DESC* pDesc, _uint elementCount);
    _uint Get_ElementCount() { return m_iElementCount; };
    const D3D11_INPUT_ELEMENT_DESC* Get_ElementDesc() { return m_pElementDesc; };

public:
    HRESULT Bind_Model(ID3D11DeviceContext* pContext);
    HRESULT Render_Model(ID3D11DeviceContext* pContext);
    CVIBuffer* Get_Buffer() { return m_pBuffer; };

public:
    virtual void Render_GUI() override;
private:
    CVIBuffer* m_pBuffer = { nullptr };
    _uint m_iElementCount = {};
    const D3D11_INPUT_ELEMENT_DESC* m_pElementDesc = { nullptr };

public:
    static CModel* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

NS_END