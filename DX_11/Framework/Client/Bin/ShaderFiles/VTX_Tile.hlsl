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
    float4 viewPos = mul(worldPos, matView);
    float4 projPos = mul(viewPos, matProjection);


    Out.vPosition = projPos;
    Out.vWorldPos = worldPos;
    Out.vTexcoord = In.vTexcoord;
    Out.iMtlType = In.iMtlType;
    Out.vNormal = mul(vector(In.vNormal, 0.f), instWorld);

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
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(matWorld[TransformIndex], matView);
    matWVP = mul(matWV, matProjection);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(vector(In.vNormal, 0.f), matWorld[TransformIndex]);
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), matWorld[TransformIndex]);
    
    return Out;
}

struct PS_INSTATNCE_IN
{
    float4 vPosition : SV_Position;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
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
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);

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
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
    return Out;
}

PS_OUT PS_TILE_INSTANCE(PS_INSTATNCE_IN In)
{
    PS_OUT Out;
    //if (In.iMtlType.y == 0.f)
    //{
    //    discard;
    //    Out.vColor = float4(0.5f, 0.f, 0.f, 0.2f);
    //    return Out;
    //}
    //
    //if (In.iMtlType.y <0.f)
    //{
    //    discard;
    //    Out.vColor = float4(0.f, 0.5f, 0.f, 0.2f);
    //    return Out;
    //}
    if (In.iMtlType.x == 0.f)
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
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y));
    vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));
    Diffuse = Grd;
    if (Diffuse.a < 0.2f)
        discard;
    Out.vDiffuse = Diffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
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
    pass Instancing
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TILE_INSTANCE();
    }
}