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
    float4 localPos = mul(float4(In.vPosition, 1.0f), instWorld);
    float4 WorldPos = mul(localPos, g_WorldMatrix);
    float4 viewPos = mul(WorldPos, matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vWorldPos = WorldPos;
    Out.vTexcoord = In.vTexcoord;
    Out.iMtlType = In.iMtlType;
    
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
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    //노멀 벡터를 월드 변환해줌
    vector vWorldNormal = mul(vector(In.vNormal, 0.f), matWorld[TransformIndex]);
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), matWorld[TransformIndex]);
    
    //빛의 방향의 반대와 월드노멀의 내적을 통해 그 각도를 구해줌 (최소 0을 내려가지 않도록)
    Out.vShade = saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vWorldNormal)), 0.f) + (vLightAmbient * vMtrlAmbient));
    vector vReflect = reflect(normalize(vLightDir), normalize(vWorldNormal));
    vector vLook = Out.vWorldPos - vCamPosition;
    
    Out.fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), fSpecularPow * 100);
    return Out;
}
struct PS_INSTATNCE_IN
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 iMtlType : TEXCOORD2;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vShade : TEXCOORD1;
    float fSpecular : TEXCOORD2;
    float4 vWorldPos : TEXCOORD3;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
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

    Out.vColor = BaseColor;
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

PS_OUT PS_TILE_INSTANCE(PS_INSTATNCE_IN In)
{
    PS_OUT Out;

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
    
    Out.vColor = Diffuse;
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
        PixelShader = compile ps_5_0 PS_BASE();
    }
  pass Edge
  {
      SetRasterizerState(RS_Default);
      SetDepthStencilState(DSS_Default, 0);
      SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_EDGE();
    }
    pass Instancing
    {
        SetRasterizerState(RS_Wireframe);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_INSTANCE();
        PixelShader = compile ps_5_0 PS_TILE_INSTANCE();
    }
}