#include "Shader_Define.hlsl"
float2 leafPalette = { 0.25,0.3};

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
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
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
    Out.vNormal  = mul(vNormal, matWorld[TransformIndex]);
    Out.vProjPos = Out.vPosition;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.2)
    {
        discard;
    }
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

    return Out;
}

PS_OUT PS_SKIN(PS_IN In)
{
    PS_OUT Out;
     
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vMtrlDiffuse -=  float4(0.06, 0.23, 0.38, 0.f);
    if (vMtrlDiffuse.a < 0.2)
    {
        discard;
    }
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

    return Out;
}

PS_OUT PS_TREE(PS_IN In)
{
    PS_OUT Out;
    
    vector vMixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vAlbGry = AlbedoGrayTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vMtrlDiffuse = g_PaletteTexture.Sample(DefaultSampler, float2((1 - vAlbGry.r), 0.6));
    
    if (vMtrlDiffuse.a < 0.2)
    {
        discard;
    }
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);


    return Out;
}

PS_OUT PS_LEAF(PS_IN In)
{
    PS_OUT Out;
    
    vector vMixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vAlbGry = AlbedoGrayTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vMtrlDiffuse = g_PaletteTexture.Sample(DefaultSampler, leafPalette * (1 - vAlbGry.r));
    vector vOpacity = OpacityTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vOpacity.r > 0)
    {
        Out.vDiffuse = vMtrlDiffuse;
        Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
        Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

    }
    else
        discard;

    return Out;
}

PS_OUT PS_FORCE(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDiffuse = (1.f,1.f,1.f,1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}



struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out;
    
    matrix matWV, matWVP;
    matWV = mul(matWorld[TransformIndex], matShadowView);
    matWVP = mul(matWV, matShadowProjection);
    
    float fWeightW = 1.0 - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    float4x4 BoneMatrix =
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.x].BoneMat * In.vBlendWeight.x +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.y].BoneMat * In.vBlendWeight.y +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.z].BoneMat * In.vBlendWeight.z +
        g_BoneMatrices[SkinningOffset + In.vBlendIndex.w].BoneMat * fWeightW;
    
    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN_SHDOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    vector vShadow : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHDOW In)
{
    PS_OUT_SHADOW Out;
 
    Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zShadowFar, 0.f, 0.f);
  
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
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass SkinShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SKIN();
    } 

    pass Tree
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TREE();
    }
    pass Leaf
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LEAF();
    }
    pass ForceSee
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FORCE();
    }
    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }
}

