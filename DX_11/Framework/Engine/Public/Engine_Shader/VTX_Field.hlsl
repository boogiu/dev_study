#include "Shader_Define.hlsl"

float fWaveTime;
float fFade;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float3 worldPos = mul(float4(In.vPosition, 1.f), matWorld[TransformIndex]).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vTexcoord = In.vTexcoord;
    
    //노멀 벡터를 월드 변환해줌
    Out.vNormal = mul(vector(In.vNormal, 0.f), matWorld[TransformIndex]);
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), matWorld[TransformIndex]);
    Out.vProjPos = Out.vPosition;
   
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), matWorld[TransformIndex])).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    return Out;
}

VS_OUT VS_NOCURVE_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float3 worldPos = mul(float4(In.vPosition, 1.f), matWorld[TransformIndex]).xyz;

    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vTexcoord = In.vTexcoord;
    
    //노멀 벡터를 월드 변환해줌
    Out.vNormal = mul(vector(In.vNormal, 0.f), matWorld[TransformIndex]);
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), matWorld[TransformIndex]);
    Out.vProjPos = Out.vPosition;
   
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), matWorld[TransformIndex])).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    return Out;
}



struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
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
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vDiffuse = vMtrlDiffuse;
    
    if (Out.vDiffuse.a < 0.2)
    {
        discard;
    }
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}

PS_OUT PS_BASE(PS_IN In)
{
    PS_OUT Out;

    float2 worldSize = vMax - vMin;
    float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;
    float2 uv = frac(WorldUV * repeatCount);

    vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);

    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y));

    vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));

    Out.vDiffuse = Grd;
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    
    return Out;
}

PS_OUT PS_EDGE(PS_IN In)
{
    PS_OUT Out;
  
    float2 worldSize = vMax - vMin;
    float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;

    float2 uv = frac(WorldUV * repeatCount);

    vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);

    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y));
    
    vector Opacity = OpacityTexture.Sample(LinearSampler, In.vTexcoord);

    vector Grd = (Opacity.a) * (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));

    if (Grd.a < 0.2f)
        discard;
    Out.vDiffuse = Grd;
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}

PS_OUT PS_RIVER(PS_IN In)
{
    PS_OUT Out;
  
    vector diffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    vector Mix = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);

    Out.vDiffuse = float4(Mix.a * diffuse.xyz, diffuse.a);
    Out.vDiffuse += float4((1 - Mix.a) * ShallowColor, 1);
    
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    
    return Out;
}


PS_OUT PS_WAVE(PS_IN In)
{
    PS_OUT Out;

    vector vIdxMap = IndexMap.Sample(DefaultSampler, In.vTexcoord);
    float4 waterColor = float4(vIdxMap.b * DeepColor + (1 - vIdxMap.b) * ShallowColor, 1.f);
    
    float u = frac(In.vTexcoord.x); // X 래핑
    float v = clamp(In.vTexcoord.y + fWaveTime, 0, 1); // Y 클램프
    vector vAlbGry = AlbedoGrayTexture.Sample(PointClampSampler, float2(u, In.vTexcoord.y + fWaveTime*2));
    vector vAlbOry = AlbedoOryTexture.Sample(PointClampSampler, float2(In.vTexcoord.x,  fWaveTime));

    waterColor += vAlbGry;
    
    if (vAlbGry.a < 0.2f)
        discard;
    
    waterColor.a = fWaveTime;
    Out.vDiffuse = waterColor;
    
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, float2( In.vTexcoord.x + fWaveTime,v));
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0, 1);
    return Out;
}

PS_OUT PS_SEA_WAVE(PS_IN In)
{
    PS_OUT Out;
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, float2(In.vTexcoord.x, 
    In.vTexcoord.y + fWaveTime));
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    Out.vDiffuse = float4(ShallowColor,1.f);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0, 1);
    return Out;
}

PS_OUT PS_BEACH(PS_IN In)
{
    PS_OUT Out;
    vector Index = IndexMap.Sample(DefaultSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    vector Diffuse = DiffuseTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    vector AlbGry = AlbedoGrayTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    
    vector color = Diffuse;
    Out.vDiffuse = color;
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0, 1);
    return Out;
}


PS_OUT PS_SAND(PS_IN In)
{
    PS_OUT Out;
    vector Diffuse = DiffuseTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    vector AlbOry = AlbedoOryTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    
    Out.vDiffuse = Diffuse + AlbOry * 0.4f;
    vector vNormalDesc = NormalTexture.Sample(LinearSampler, float2(In.vTexcoord.x, In.vTexcoord.y));
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 1.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0, 1);
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
    
    float3 worldPos = mul(float4(In.vPosition, 1.f), matWorld[TransformIndex]).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(float4(worldPos, 1.f), matShadowView);
    float4 projPos = mul(viewPos, matShadowProjection);
    
    Out.vPosition = projPos;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}
struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};
struct PS_OUT_SHADOW
{
    vector vShadow : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
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
    pass Force_See
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_NOCURVE_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Base
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BASE();
    }

    pass Edge
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EDGE();
    }

    pass River
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_RIVER();
    }
    pass Wave
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_WAVE();
    }
    pass SeaWave
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SEA_WAVE();
    }
    pass Beach
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BEACH();
    }
    pass Sand
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SAND();
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