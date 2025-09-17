#ifndef __SHADER_DEFINE_HLSL__
#define __SHADER_DEFINE_HLSL__
#pragma pack_matrix(row_major)

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

texture2D g_DiffuseTexture : register(t0);

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

#endif // __SHADER_DEFINE_HLSL__