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
    float2 vTexcoord : TEXCOORD0;
    float4 vShade : TEXCOORD1;
    float fSpecular : TEXCOORD2;
    float4 vWorldPos : TEXCOORD3;
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
    Out.vWorldPos = mul(vPosition, matWorld[TransformIndex]);
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

PS_OUT PS_SKIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vMtrlDiffuse -=  float4(0.06, 0.23, 0.38, 0.f);
    if (vMtrlDiffuse.a < 0.3)
    {
        discard;
    }
    Out.vColor = vMtrlDiffuse;
    
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

PS_OUT PS_TREE(PS_IN In)
{
    PS_OUT Out;
    
    vector vMixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vAlbGry = AlbedoGrayTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vMtrlDiffuse = g_PaletteTexture.Sample(DefaultSampler, float2((1 - vAlbGry.r), 0.6));
    
     Out.vColor =vMtrlDiffuse;
    
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
        Out.vColor = vMtrlDiffuse;

    }
    else
        discard;


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

    pass SkinShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_SKIN();
    } 

    pass ForceBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
    PixelShader = compile ps_5_0 PS_BLEND();
    }

    pass Tree
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_TREE();
    }
    pass Leaf
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_LEAF();
    }
}

