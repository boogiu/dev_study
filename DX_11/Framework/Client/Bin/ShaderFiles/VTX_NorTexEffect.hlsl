#include "Shader_Define.hlsl"
float fElapsedTime;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    matrix matWV = mul(matWorld[TransformIndex], matView);
    matrix matWVP = mul(matWV, matProjection);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    float3 worldNormal = mul(float4(In.vNormal, 0.f), matWorld[TransformIndex]).xyz;
    Out.vNormal = normalize(worldNormal);

    Out.vProjPos = Out.vPosition;

    float3 worldTangent = mul(float4(1.f, 0.f, 0.f, 0.f), matWorld[TransformIndex]).xyz;
    Out.vTangent = normalize(worldTangent * -1);

    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    float4 vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 vAvlbedoGry= AlbedoGrayTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vAvlbedoGry.a < fElapsedTime)
        discard;

    vMtrlDiffuse += vAvlbedoGry.r;
    float3 vNormalMap = NormalTexture.Sample(DefaultSampler, In.vTexcoord).xyz * 2.f - 1.f;

    float3x3 TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);
    float3 vWorldNormal = mul(vNormalMap, TBN);

    Out.vDiffuse = 1-vMtrlDiffuse;
    Out.vNormal = float4(normalize(vWorldNormal) * 0.5f + 0.5f, 1.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

    return Out;
}

technique11 DefaultTechnique
{
    pass Opaque
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
