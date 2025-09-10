#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CPipeLine :
    public CBase
{
    struct FrameBuffer
    {
        _float4x4 matView;
        _float4x4 matProjection;
        _float4x4 matOrthograph;
        _float4     vCamPosition;
    };

    struct LightBuffer
    {
        _float4 vLightDir;
        _float4 vLightDiffuse;
        _float4 vLightAmbient;
        _float4 vLightSpecular;
    };

    struct ObjectBuffer
    {
        _float4x4 matWorld;
    };

private:
    CPipeLine();
   virtual ~CPipeLine() DEFAULT;

public:
    HRESULT Initialize(ID3D11Device* pDevice);
    HRESULT Update_FrameBuffer(ID3D11DeviceContext* pContext);
    HRESULT Update_LightBuffer(ID3D11DeviceContext* pContext);
    HRESULT Update_ObjectBuffer(ID3D11DeviceContext* pContext, _float4x4* pMatrix);

public:
    ID3D11Buffer* Get_FrameBuffer() { return m_pDeviceFrameBuffer; };
    ID3D11Buffer* Get_ObjectBuffer() { return m_pDeviceObjectBuffer; };
    ID3D11Buffer* Get_LightBuffer() { return m_pDeviceLightBuffer; };

private:
    ID3D11Buffer* m_pDeviceFrameBuffer = {nullptr};
    ID3D11Buffer* m_pDeviceLightBuffer = {nullptr};
    ID3D11Buffer* m_pDeviceObjectBuffer = {nullptr};

public:
    static CPipeLine* Create(ID3D11Device* pDevice);
    virtual void Free() override;
};
NS_END