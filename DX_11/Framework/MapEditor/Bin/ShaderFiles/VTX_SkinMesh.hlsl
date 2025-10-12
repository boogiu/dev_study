#include "Shader_Define.hlsl"

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vShade : TEXCOORD1;
    float fSpecular : TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(matWorld[TransformIndex], matView);
    matWVP = mul(matWV, matProjection);
    
    float fWeightW = 1.0 - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    float4x4 BoneMatrix =
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.x].BoneMat * In.vBlendWeight.x +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.y].BoneMat * In.vBlendWeight.y +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.z].BoneMat * In.vBlendWeight.z +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.w].BoneMat * fWeightW;
    
    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    float4 vWorldPos = mul(vPosition, matWorld[TransformIndex]);
    float4 vWorldNormal = mul(vNormal, matWorld[TransformIndex]);
    
    //빛의 방향의 반대와 월드노멀의 내적을 통해 그 각도를 구해줌 (최소 0을 내려가지 않도록)
    Out.vShade = saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vWorldNormal)), 0.f) + (vLightAmbient * vMtrlAmbient));
    float4 vReflect = reflect(normalize(vLightDir), normalize(vWorldNormal));
    float4 vLook = vWorldPos - vCamPosition;
    
    Out.fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), fSpecularPow * 100);
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vShade : TEXCOORD1;
    float fSpecular : TEXCOORD2;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
   //빛의 색상 * 빛의 강도 * 텍스처 색깔
    Out.vColor = vLightDiffuse * vMtrlDiffuse * In.vShade +
       (vLightSpecular * vMtrlSpecular) * In.fSpecular;

    return Out;
}

PS_OUT PS_BLEND(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
   //빛의 색상 * 빛의 강도 * 텍스처 색깔
    Out.vColor = vMtrlDiffuse;
    
    Out.vColor.a = 0.4f;
    Out.vColor.b += 0.05f;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Opaque
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass ForceBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_BLEND();
    }
}

