#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class CPipeLine :
    public CBase
{
    struct alignas(16)  FrameBuffer
    {
        _float4x4 matView;
        _float4x4 matProjection;
        _float4x4 matViewInverse;
        _float4x4 matProjectionInverse;
        _float4x4 matOrthograph;
        _float4 vCamPosition;
        _float3 CameraForward;
        _float zFar;
    };


    struct alignas(16)  ShadowBuffer
    {
        _float4x4 matShadowView;
        _float4x4 matShadowProjection;
        _float4 vShadowPosition;
        _float zShadowFar;
        _float3 ShadowPadding;
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
    HRESULT Initialize(ID3D11Device* pDevice, class CRenderSystem* pSystem);
public:
    _bool isVisible(MINMAX_BOX minMax, _fmatrix worldTransform);
public:
    HRESULT Update_FrameBuffer(ID3D11DeviceContext* pContext);
    HRESULT Update_CurvedBuffer(ID3D11DeviceContext* pContext);
    HRESULT Update_ShadowBuffer(ID3D11DeviceContext* pContext);
    void Update_Frustum();

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
    ID3D11Buffer* Get_ShadowBuffer() { return m_pDeviceShadowBuffer; };

    ID3D11Buffer* Get_ObjectArrayBuffer() { return m_pDeviceObjectBuffer; };
    ID3D11ShaderResourceView* Get_SkinningResource() { return m_pSkinningResource; };
   
    HRESULT Bind_Light(class CShader* pShader, class CVIBuffer* pBuffer, ID3D11DeviceContext* pContext);

private:
    ID3D11Buffer* m_pDeviceFrameBuffer = {nullptr};
    ID3D11Buffer* m_pDeviceShadowBuffer = {nullptr};
    
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
    class CRenderSystem* m_pSystem = { nullptr };

    BoundingFrustum m_Frustum;
public:
    static CPipeLine* Create(ID3D11Device* pDevice, class CRenderSystem* pSystem);
    virtual void Free() override;
};
NS_END