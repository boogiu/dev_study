#ifndef __SHADER_DEFINE_HLSL__
#define __SHADER_DEFINE_HLSL__

#pragma pack_matrix(row_major)

#include "Shader_State.hlsl"

cbuffer FrameBuffer : register(b0)
{
    matrix matView;
    matrix matProjection;
    matrix matOrthograph;
    vector vCamPosition;
};

cbuffer LightBuffer : register(b1)
{
    vector vLightDir;
    vector vLightDiffuse;
    vector vLightAmbient;
    vector vLightSpecular;
};

cbuffer ObjectBuffer : register(b2)
{
    matrix matWorld;
}

cbuffer SkinningBuffer : register(b3)
{
    matrix BoneMatrices[512];
}

cbuffer MaterialBuffer : register(b4)
{
    float4 vMtrlDiffuse;
    float4 vMtrlAmbient;
    float4 vMtrlSpecular;
    float fSpecularPow;
};

cbuffer TileBuffer : register(b5)
{
    int TileIndex; 
    int NeighborIndex[4];
    int PaletteTexSize;
}

SamplerState DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState PointSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

texture2D g_PaletteTexture  : register(t0);
texture2D g_EdgeTexture      : register(t1);
texture2D g_DiffuseTexture  : register(t2);

#endif // __SHADER_DEFINE_HLSL__