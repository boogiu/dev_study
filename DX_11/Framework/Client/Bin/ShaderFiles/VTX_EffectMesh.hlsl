#include "Shader_Define.hlsl"
float fElapsedTime;
float fRadian;
float2 vGradation;
float4x4 g_WorldMatrix;

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
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    float3 worldPos = mul(float4(In.vPosition, 1.f), matWorld[TransformIndex]).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vWorldPos = float4(worldPos, 1.f);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(vector(In.vNormal, 0.f), matWorld[TransformIndex]);
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
    float4 vWorldPos : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
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
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
    float4 vMtrl = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMtrl.r < fElapsedTime)
        discard;
    
    Out.vDiffuse = 1.f;
    Out.vNormal = vector(In.vNormal.xyz* 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
   return Out;
}

PS_OUT PS_SPLASH(PS_IN In)
{
    PS_OUT Out;
    
    float4 vMtrl = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
   
    if (vMtrl.a < fElapsedTime)
        discard;
    
    Out.vDiffuse = 1.f;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
     Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
   return Out;
}


PS_OUT PS_RUBBLE(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vDiffuse = vMtrlDiffuse;
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}


PS_OUT PS_GRAD(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = GradationTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vDiffuse = vMtrlDiffuse;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}



PS_OUT PS_PALETTE(PS_IN In)
{
    PS_OUT Out;
  
    float2 worldSize = vMax - vMin;
    float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;

    float2 uv = frac(WorldUV * repeatCount);

    vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);

    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y + 0.1f));
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y));
    
    vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));

    if (Grd.a < 0.2f)
        discard;
    Out.vDiffuse = Grd;
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


struct VS_INSTANCE_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    
    float4 iRight : INSTANCE0;
    float4 iUp : INSTANCE1;
    float4 iLook : INSTANCE2;
    float4 iTrans : INSTANCE3;
    float4 iColor : INSTANCE4;
    float2 iLifeTime : INSTANCE5;
};

struct VS_INSTANCE_OUT
{
    float4 vPosition : SV_Position;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 iColor : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float2 iLifeTime : TEXCOORD3;
};

VS_INSTANCE_OUT VS_INSTANCE(VS_INSTANCE_IN In)
{
    VS_INSTANCE_OUT Out;

    row_major float4x4 instWorld = float4x4(In.iRight, In.iUp, In.iLook, In.iTrans);

    float4 instancePos = mul(float4(In.vPosition, 1.0f), instWorld);
    float4 worldPos =mul(instancePos, g_WorldMatrix);

    float3 toObj = worldPos.xyz - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;

    float4 viewPos = mul(worldPos, matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.iColor = In.iColor;
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    Out.iLifeTime = In.iLifeTime;
    
    float3 worldNormal = mul(float4(In.vNormal, 0), mul(instWorld, g_WorldMatrix)).xyz;
    Out.vNormal = normalize(worldNormal);
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), mul(instWorld,g_WorldMatrix))).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));

    return Out;
}


struct PS_INSTANCE_IN
{
    float4 vPosition : SV_Position;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 iColor : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    float2 iLifeTime : TEXCOORD3;
};

PS_OUT PS_INSTANCE(PS_INSTANCE_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMixture = MixtureTexture.Sample(LinearSampler, In.vTexcoord);
    
    //if (vMtrlDiffuse.a < In.iLifeTime.x / In.iLifeTime.y)
    //    discard;
    
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);

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
    pass Splash
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SPLASH();
    }
    pass Rubble
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_RUBBLE();
    }
    pass Palette
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PALETTE();
    }
    pass Gradation
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GRAD();
    }

    pass Particle
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_INSTANCE();
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

