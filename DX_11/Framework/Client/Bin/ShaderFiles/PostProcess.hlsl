#include "Shader_Define.hlsl"
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

    Out.vWorldPos = float4(0, 0, 0, 1); // 의미 없음 (GS가 재정의)

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

    float4 p0 = float4(-1, 1, 0, 1); // Left Top
    float4 p1 = float4(1, 1, 0, 1); // Right Top
    float4 p2 = float4(1, -1 +fLifeTime, 0, 1); // Right Bottom
    float4 p3 = float4(-1, -1 + fLifeTime, 0, 1); // Left Bottom

    v[0].vPosition = p0;
    v[0].vTexcoord = float2(0, 0);
    v[1].vPosition = p1;
    v[1].vTexcoord = float2(1, 0);
    v[2].vPosition = p2;
    v[2].vTexcoord = float2(1, 1);
    v[3].vPosition = p3;
    v[3].vTexcoord = float2(0,1);

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

struct PS_UI
{
    vector vUIText : SV_TARGET0;
};

PS_OUT PS_CRAFT(PS_IN In)
{
    PS_OUT Out;

    float2 uv = In.vTexcoord;
    uv.x *= 4;
    float mask = MixtureTexture.Sample(LinearSampler, uv).a;
    
    if (mask > 0.f)
        Out.vColor = float4(0, 0, 0, 0);
    else
        discard;

    return Out;
}


technique11 DefaultTechnique
{
    pass Transition_Craft
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_CRAFT();
    }  
}

