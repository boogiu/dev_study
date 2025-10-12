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

    struct ObjectBufferArray {
        ObjectBuffer Objects[1024]; 
    };

    struct SkinningBuffer
    {
        _float4x4 SkinningBuffer[g_iMaxNumBones];
    };

    struct  TileBuffer
    {
        _float2 PalettePixel;       
        _int PaletteIndex;            
        _int PaletteEdge;             
        _int MaskPalette;            
        _int _Padding[3];            
    };

private:
    CPipeLine();
   virtual ~CPipeLine() DEFAULT;

public:
    HRESULT Initialize(ID3D11Device* pDevice);
    HRESULT Update_FrameBuffer(ID3D11DeviceContext* pContext);
    HRESULT Update_LightBuffer(ID3D11DeviceContext* pContext);

    _uint Write_ObjectData(const _float4x4& worldMatrix);
    HRESULT Begin_ObjectBuffer(ID3D11DeviceContext* pContext);
    HRESULT End_ObjectBuffer(ID3D11DeviceContext* pContext);

    _uint Write_SkinningBuffer(const vector<_float4x4>& BoneMatrices);
    HRESULT Begin_SkinningBuffer(ID3D11DeviceContext* pContext);
    HRESULT End_SkinningBuffer(ID3D11DeviceContext* pContext);

    HRESULT Bind_PaletteTexture(class CShader* pShader);
    HRESULT Add_Palette(const string& ConstantName, class CTexture* pTexture);

public:
    ID3D11Buffer* Get_FrameBuffer() { return m_pDeviceFrameBuffer; };
    ID3D11Buffer* Get_LightBuffer() { return m_pDeviceLightBuffer; };

    ID3D11Buffer* Get_ObjectArrayBuffer() { return m_pDeviceObjectBuffer; };
    ID3D11ShaderResourceView* Get_SkinningResource() { return m_pSkinningResource; };

private:
    ID3D11Buffer* m_pDeviceFrameBuffer = {nullptr};
    ID3D11Buffer* m_pDeviceLightBuffer = {nullptr};
    
    _int m_ObjectBufferCount = {};
    ObjectBufferArray* m_pObjectBufferArray = { nullptr };
    ID3D11Buffer* m_pDeviceObjectBuffer = { nullptr };
    D3D11_MAPPED_SUBRESOURCE m_mappedObjectBuffer = {};

    _uint m_SkinningOffset = {};
    _float4x4* m_pSkinningArray = nullptr;
    ID3D11Buffer* m_pDeviceSkinningBuffer = { nullptr };
    ID3D11ShaderResourceView* m_pSkinningResource = { nullptr };
    D3D11_MAPPED_SUBRESOURCE m_mappedSkinningBuffer = {};

    /*ÆÈ·¹Æ® µî·Ï*/
    unordered_map<string, class CTexture*> m_Palette;

public:
    static CPipeLine* Create(ID3D11Device* pDevice);
    virtual void Free() override;
};
NS_END