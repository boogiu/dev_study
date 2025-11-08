#include "Shader_Define.hlsl"

float4 blushColor = float4(1.0, 0.55, 0.55, 1.f);
float4 eyeColor = float4(0.15, 0.25, 0.45, 1.f);
float4 SkinColor = float4(242.f / 255.f, 228.f / 255.f, 206.f / 255.f, 1.f);
float4 HairColor = float4(125.f / 255.f, 87.f / 255.f, 31.f / 255.f, 1.f);

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
    Out.vNormal = mul(vNormal, matWorld[TransformIndex]);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

float3 HSVtoRGB(float3 hsv)
{
    float H = hsv.x;
    float S = hsv.y;
    float V = hsv.z;

    float C = V * S;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float3 rgb;

    if (H < 60)
        rgb = float3(C, X, 0);
    else if (H < 120)
        rgb = float3(X, C, 0);
    else if (H < 180)
        rgb = float3(0, C, X);
    else if (H < 240)
        rgb = float3(0, X, C);
    else if (H < 300)
        rgb = float3(X, 0, C);
    else
        rgb = float3(C, 0, X);

    float m = V - C;
    return rgb + m;
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
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f,1.f);
    
    return Out;
}

PS_OUT PS_SKIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
   
    vMtrlDiffuse -= (1 - SkinColor);
      if (vMtrlDiffuse.a < 0.2)
    {
        discard;
    }
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

    return Out;
}

PS_OUT PS_EYE(PS_IN In)
{
    PS_OUT Out;

    float4 tex = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 Mixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
     float a = tex.a;
    float3 baseCol = tex.rgb;

    float3 color = 0;

    // 알파가 거의 없고, RGB도 거의 없으면 피부색
    if (a < 0.05 && baseCol.r< 0.05)
    {
        color = SkinColor.rgb *Mixture.r;
    }
    //  알파가 있고, RGB가 거의 없으면 -> 눈 영역
    else if (a > 0.05 && baseCol.r < 0.1)
    {
        // 알파 강도에 따라 눈동자 색 보간
        color = lerp(baseCol, eyeColor.rgb, saturate(a * 1.0));
    }
    // 나머지는 텍스처 색 그대로
    else
    {
        color = baseCol;
    }
    
  
    Out.vDiffuse = float4(color, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}


PS_OUT PS_MOUTH(PS_IN In)
{
    PS_OUT Out;
    
    float4 tex = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float a = tex.a;
    float3 baseCol = tex.rgb;

    float3 color = 0;
    if (baseCol.r < 0.1f)
    {
        color = SkinColor;
    }
    else
    {
        color = baseCol;
    }
    
    Out.vDiffuse = float4(color, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
   
    return Out;
}


PS_OUT PS_CHEEK(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vMtrlDiffuse = lerp(SkinColor, blushColor, 1-vMtrlDiffuse.b);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
   
    return Out;
}

PS_OUT PS_HAIR(PS_IN In)
{
    PS_OUT Out;
    
    Out.vDiffuse = HairColor;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
 
    return Out;
}

PS_OUT PS_HAIRSKIN(PS_IN In)
{
    PS_OUT Out;
    
    Out.vDiffuse = HairColor*0.5f + SkinColor*0.5f;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}

PS_OUT PS_CLOTH(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
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
    pass EyeShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EYE();
    }
    pass MouthShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MOUTH();
    }
    pass    CheekShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHEEK();
    }
    pass HairShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HAIR();
    }
    pass HairSkinShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HAIRSKIN();
    }

    pass ClothShader
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CLOTH();
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

