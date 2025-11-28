#include "Shader_Define.hlsl"

float4x4 g_WorldMatrix;

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
    float4 iMtlType : INSTANCE4;
    float4 fCornerHeight : INSTANCE5;
};


struct VS_INSTANCE_OUT
{
    float4 vPosition : SV_Position;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
    float4 vProjPos : TEXCOORD3;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_INSTANCE_OUT VS_INSTANCE(VS_INSTANCE_IN In)
{
    VS_INSTANCE_OUT Out = (VS_INSTANCE_OUT) 0;
    float2 pos = In.vPosition.xz; // 로컬 좌표 (0~1 기준)

    float height = 0.0f;

// 왼쪽 위
    if (pos.x < 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.x;
// 오른쪽 위
    else if (pos.x >= 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.y;
// 오른쪽 아래
    else if (pos.x >= 0.5 && pos.y <= 0.5)
        height = In.fCornerHeight.z;
// 왼쪽 아래
    else
        height = In.fCornerHeight.w;

    float3 localPos = In.vPosition;
    localPos.y += height;

    row_major float4x4 instWorld = float4x4(In.iRight, In.iUp, In.iLook, In.iTrans);
    float4 worldPos = mul(float4(localPos, 1.0f), instWorld);
    float3 toObj = worldPos.xyz - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;

    float4 viewPos = mul(worldPos, matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vWorldPos = worldPos;
    Out.vTexcoord = In.vTexcoord;
    Out.iMtlType = In.iMtlType;
    Out.vNormal = mul(vector(In.vNormal, 0.f), instWorld);
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), instWorld)).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    return Out;
}


VS_INSTANCE_OUT VS_NOCURVE_INSTANCE(VS_INSTANCE_IN In)
{
    VS_INSTANCE_OUT Out = (VS_INSTANCE_OUT) 0;
    float2 pos = In.vPosition.xz; // 로컬 좌표 (0~1 기준)

    float height = 0.0f;

// 왼쪽 위
    if (pos.x < 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.x;
// 오른쪽 위
    else if (pos.x >= 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.y;
// 오른쪽 아래
    else if (pos.x >= 0.5 && pos.y <= 0.5)
        height = In.fCornerHeight.z;
// 왼쪽 아래
    else
        height = In.fCornerHeight.w;

    float3 localPos = In.vPosition;
    localPos.y += height;

    row_major float4x4 instWorld = float4x4(In.iRight, In.iUp, In.iLook, In.iTrans);
    float4 worldPos = mul(float4(localPos, 1.0f), instWorld);
  
    float4 viewPos = mul(worldPos, matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vWorldPos = worldPos;
    Out.vTexcoord = In.vTexcoord;
    Out.iMtlType = In.iMtlType;
    Out.vNormal = mul(vector(In.vNormal, 0.f), instWorld);
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), instWorld)).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    return Out;
}

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
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float3 localPos = In.vPosition;
    float3 worldPos = mul(float4(localPos, 1.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;

    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vProjPos = projPos;
    Out.vWorldPos = float4(worldPos, 1.f);

    Out.vTexcoord = In.vTexcoord;

    float3 worldNormal = mul(float4(In.vNormal, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    Out.vNormal = float4(normalize(worldNormal), 0.f);

    float3 worldTangent = mul(float4(In.vTangent, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    worldTangent *= -1; 
    Out.vTangent = normalize(worldTangent);

    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));

    return Out;
}

VS_OUT VS_NOCURVE(VS_IN In)
{
    VS_OUT Out;

    float3 localPos = In.vPosition;
    float3 worldPos = mul(float4(localPos, 1.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vProjPos = projPos;
    Out.vWorldPos = float4(worldPos, 1.f);

    Out.vTexcoord = In.vTexcoord;

    float3 worldNormal = mul(float4(In.vNormal, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    Out.vNormal = float4(normalize(worldNormal), 0.f);

    float3 worldTangent = mul(float4(In.vTangent, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    worldTangent *= -1;
    Out.vTangent = normalize(worldTangent);

    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));

    return Out;
}

struct PS_INSTATNCE_IN
{
    float4 vPosition : SV_Position;
    float3 vNormal : NORMAL;
   float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
    float4 vProjPos : TEXCOORD3;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
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

PS_OUT PS_BASE(PS_IN In)
{
    PS_OUT Out;

    vector Diffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector Mixture = MixtureTexture.Sample(LinearSampler, In.vTexcoord);
    vector Gradation = GradationTexture.Sample(LinearSampler, PalettePixel);

    float hasDiffuse = step(0.01f, Diffuse.a); // 알파 거의 0이면 없음
    float hasGradation = step(0.01f, Gradation.a); // 알파 거의 0이면 없음

    vector BaseColor = Diffuse;

    if (hasDiffuse < 0.2f && hasGradation > 0.2f)
    {
        BaseColor = Gradation;
    }
    else if (hasDiffuse > 0.2 && hasGradation > 0.2f)
    {
        BaseColor = Diffuse;
    }
    else if (hasDiffuse < 0.2f && hasGradation < 0.2f)
    {
        discard;
    }
    
    vector Mtrl = BaseColor;

    if (Mtrl.a < 0.2f)
        discard;

    Out.vDiffuse = BaseColor;
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);\

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
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
     vNormal = In.vNormal.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}

PS_OUT PS_TILE_INSTANCE(PS_INSTATNCE_IN In)
{
    PS_OUT Out;
   
    if (In.iMtlType.x ==  0.f)
    {
        discard;
    }
    
    vector Diffuse;
    float2 worldSize = vMax - vMin;
    float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;

    float2 uv = frac(WorldUV * repeatCount);
    vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);
    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, 
    float2(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y));
    vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));
    Diffuse = Grd;
    
    if (Diffuse.a < 0.2f)
        discard;
    
    Out.vDiffuse = Diffuse;
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    
     return Out;
}

PS_OUT PS_TILE_INSTANCE_DBUG(PS_INSTATNCE_IN In)
{
    PS_OUT Out;
       vector Diffuse;
    
    //if (In.iMtlType.y == -1.f)
    //{
    //    Diffuse = float4(0.6, 0.6, 0.2, 0.8);
    //}
    //    
    //else if(In.iMtlType.y == -2.f)
    //{
    //    Diffuse = float4(0.f, 0.6, 0.6, 0.8);
    //}
    //else if (In.iMtlType.x == 1.f)
    //{
    //    Diffuse = float4(1.6, 0.6, 0.2, 0.8);
    //}
    //else
    //    discard;
    
    Out.vDiffuse = Diffuse;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    
    return Out;
}

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_INSTANCE_SHADOW(VS_INSTANCE_IN In)
{
    VS_OUT_SHADOW Out;
    float2 pos = In.vPosition.xz; // 로컬 좌표 (0~1 기준)

    float height = 0.0f;

// 왼쪽 위
    if (pos.x < 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.x;
// 오른쪽 위
    else if (pos.x >= 0.5 && pos.y > 0.5)
        height = In.fCornerHeight.y;
// 오른쪽 아래
    else if (pos.x >= 0.5 && pos.y <= 0.5)
        height = In.fCornerHeight.z;
// 왼쪽 아래
    else
        height = In.fCornerHeight.w;

    float3 localPos = In.vPosition;
    localPos.y += height;

    row_major float4x4 instWorld = float4x4(In.iRight, In.iUp, In.iLook, In.iTrans);
    float4 worldPos = mul(float4(localPos, 1.0f), instWorld);
    float3 toObj = worldPos.xyz - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(worldPos, matShadowView);
    float4 projPos = mul(viewPos, matShadowProjection);
    
    Out.vPosition = projPos;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out;
    float4 worldPos = mul(float4(In.vPosition, 1.0f), ObjectBufferArray[TransformIndex].Transform);
    float3 toObj = worldPos.xyz - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(worldPos, matShadowView);
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

    pass NoCurve
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_NOCURVE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EDGE();
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

    pass InstanceShadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_INSTANCE_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }
    pass Instancing
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TILE_INSTANCE();
    }
    pass InstancingNoCurve
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_NOCURVE_INSTANCE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TILE_INSTANCE_DBUG();
    }
}