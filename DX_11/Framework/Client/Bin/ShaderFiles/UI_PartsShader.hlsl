#include "Shader_Define.hlsl"

Texture2D UI_MaskTexture;
Texture2D UI_FrameTexture;
Texture2D UI_GradationTexture;
float4x4 transformMatrix;
float4x4 matOrtho;
float2 PartSize;
float fLifeTime;

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vWorldPos : POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), transformMatrix);
   
    return Out;
}

struct GS_IN
{
    float4 vWorldPos : POSITION;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> triStream)
{
    GS_OUT v[4];

    float3 worldPos = In[0].vWorldPos.xyz;
    
    float3 right = normalize(transformMatrix[0].xyz);
    float3 up = normalize(transformMatrix[1].xyz);
    float scaleX = length(transformMatrix[0].xyz);
    float scaleY = length(transformMatrix[1].xyz);

    float3 offsetRight = right * (PartSize.x * 0.5f);
    float3 offsetUp = up * (PartSize.y * 0.5f);

    // 정점 4개 위치 계산 (월드 기준)
    float3 p0 = worldPos + (-offsetRight + offsetUp);
    float3 p1 = worldPos + (offsetRight + offsetUp);
    float3 p2 = worldPos + (offsetRight - offsetUp);
    float3 p3 = worldPos + (-offsetRight - offsetUp);
    
    // 직교 투영 사용
    v[0].vPosition = mul(float4(p0, 1.f), matOrtho);
    v[0].vTexcoord = float2(0, 0);

    v[1].vPosition = mul(float4(p1, 1.f), matOrtho);
    v[1].vTexcoord = float2(1, 0);

    v[2].vPosition = mul(float4(p2, 1.f), matOrtho);
    v[2].vTexcoord = float2(1, 1);

    v[3].vPosition = mul(float4(p3, 1.f), matOrtho);
    v[3].vTexcoord = float2(0, 1);

    triStream.Append(v[0]);
    triStream.Append(v[1]);
    triStream.Append(v[2]);
    triStream.RestartStrip();

    triStream.Append(v[0]);
    triStream.Append(v[2]);
    triStream.Append(v[3]);
    triStream.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vDiffuse = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    if (vDiffuse.a < 0.1f)
    {
        discard;
    }
        
    
    vDiffuse.rgb *= vDiffuse.a; // premultiply 보정
    Out.vColor = vDiffuse;
    if (Out.vColor.a < 0.1f)
        discard;
    return Out;
}

PS_OUT PS_MASKING_UI(PS_IN In)
{
    PS_OUT Out;
    
    vector vDiffuse = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    vDiffuse.rgb *= vDiffuse.a; // premultiply 보정

    float MaskScale = 1.3f;
    float2 uv = In.vTexcoord;
    float2 scaledUV = ((uv - 0.5f) / MaskScale) + 0.5f;

    vector vMasking = UI_MaskTexture.Sample(PointClampSampler, scaledUV);
    vector vFrame = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vFrame.a ==0.f)
        discard;
    
    Out.vColor = vDiffuse - vDiffuse * (vMasking.r) * 0.3;
    if (Out.vColor.a < 0.1f)
        discard;
    return Out;
}

PS_OUT PS_MASKING_INSIDE_UI(PS_IN In)
{
    PS_OUT Out;

    vector vDiffuse = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    vDiffuse.rgb *= vDiffuse.a; // premultiply 보정
    float2 uv = In.vTexcoord;
    float MaskScale = 1.6f;
    float2 scaledUV = (uv - 0.5f) / MaskScale + 0.5f;
    vector vMasking = UI_MaskTexture.Sample(PointClampSampler, In.vTexcoord);

    Out.vColor = vDiffuse - vDiffuse * (vMasking.a);
    if (Out.vColor.a < 0.1f)
        discard;
    return Out;
}

PS_OUT PS_MASKING_FlOW(PS_IN In)
{
    PS_OUT Out;

    vector vDiffuse = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    vDiffuse.rgb *= vDiffuse.a; // premultiply 보정
    float MaskScale = .7f;
    float2 uv = In.vTexcoord;
    float2 scaledUV = ((uv - 0.5f) / MaskScale) + 0.5f;
    scaledUV.x += fLifeTime*1.1f;
    vector vMasking = UI_MaskTexture.Sample(LinearSampler, scaledUV);
    vector vFrame = SpriteTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vFrame.a == 0.f)
        discard;
    
    Out.vColor = vDiffuse + vDiffuse * (vMasking.r) * 0.4;
    if (Out.vColor.a < 0.1f)
        discard;
    return Out;
}


technique11 DefaultTechnique
{
    pass Opaque
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Premultiplied, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Masking_UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Premultiplied, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MASKING_UI();
    }
    pass Inside_Empty
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Premultiplied, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MASKING_INSIDE_UI();
    }
    pass Flow_Pattern
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Premultiplied, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MASKING_FlOW();
    }
}

