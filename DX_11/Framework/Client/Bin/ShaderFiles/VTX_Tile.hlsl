#include "Shader_Define.hlsl"

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
};


struct VS_INSTANCE_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
};

VS_INSTANCE_OUT VS_INSTANCE(VS_INSTANCE_IN In)
{
    VS_INSTANCE_OUT Out = (VS_INSTANCE_OUT) 0;
    row_major float4x4 instWorld = float4x4(In.iRight, In.iUp, In.iLook, In.iTrans); // 4개가 '행'이라고 명시
    float4 worldPos = mul(float4(In.vPosition, 1.0f), instWorld);
    float4 viewPos = mul(worldPos, matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vWorldPos = worldPos;
    Out.vTexcoord = In.vTexcoord;
    Out.iMtlType = In.iMtlType;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_BASE(PS_IN In)
{
    PS_OUT Out;

   float2 worldSize = vMax - vMin;
   float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;
   float2 uv = frac(WorldUV * repeatCount);
   
   vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
   vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);
   
   vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
   vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask2.r) *  Mask2.b, PalettePixel.y));
   
   vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));

    Out.vColor = Grd;
    
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

    Out.vColor = Grd;
    return Out;
}

PS_OUT PS_TILE_INSTANCE(PS_IN In)
{
    PS_OUT Out;

    vector Diffuse;
    float2 worldSize = vMax - vMin;
    float2 WorldUV = (In.vWorldPos.xz - vMin) / worldSize;

    float2 uv = frac(WorldUV * repeatCount);
    vector Mask = g_MaskTexture.Sample(LinearSampler, uv);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, WorldUV);
        
    vector Palette = g_TilePalette.Sample(DefaultSampler, float3(PalettePixel.x, PalettePixel.y, In.iMtlType.y));
    vector Palette2 = g_TilePalette.Sample(LinearSampler, float3(PalettePixel.x + (1 - Mask2.r) * Mask2.b, PalettePixel.y, In.iMtlType.y));
    
    if (In.iMtlType.x < 0.f)
    {
       vector Grd = (Palette * (1 - Mask.a) + (Palette2) * (Mask.a));
        Diffuse = Grd;
    }
    else
    {
        Diffuse = g_TileAlbedo.Sample(LinearSampler, float3(In.vTexcoord, In.iMtlType.x));
        
    }
    
    Out.vColor = Diffuse;
    return Out;
}


technique11 DefaultTechnique
{
  pass Edge
  {
      SetRasterizerState(RS_Default);
      SetDepthStencilState(DSS_Default, 0);
      SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        PixelShader = compile ps_5_0 PS_EDGE();
    }
    pass Instancing
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        PixelShader = compile ps_5_0 PS_TILE_INSTANCE();
    }
}