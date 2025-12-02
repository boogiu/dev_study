#include "Shader_Define.hlsl"
float fLifeTime;
Texture2D g_DepthTexture;
Texture2D g_CloudTexture;

float4x4 transformMatrix;
float4x4 matViewEnvShadow;
float4x4 matProjEnvShadow;


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
    Out.vWorldPos = float4(0, 0, 0, 1);
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
void GS_SCREEN(point GS_IN In[1], inout TriangleStream<GS_OUT> triStream)
{
    GS_OUT v[4];

    float4 p0 = float4(-1, 1, 0, 1); // Left Top
    float4 p1 = float4(1, 1, 0, 1); // Right Top
    float4 p2 = float4(1, -1 , 0, 1); // Right Bottom
    float4 p3 = float4(-1, -1 , 0, 1); // Left Bottom

    v[0].vPosition = p0;
    v[0].vTexcoord = float2(0, 0);
    v[1].vPosition = p1;
    v[1].vTexcoord = float2(1, 0);
    v[2].vPosition = p2;
    v[2].vTexcoord = float2(1, 1);
    v[3].vPosition = p3;
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

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> triStream)
{
    GS_OUT v[4];

    float4 p0 = float4(-1, 1, 0, 1); // Left Top
    float4 p1 = float4(1, 1, 0, 1); // Right Top
    float4 p2 = float4(1, -1 + fLifeTime, 0, 1); // Right Bottom
    float4 p3 = float4(-1, -1 + fLifeTime, 0, 1); // Left Bottom

    v[0].vPosition = p0;
    v[0].vTexcoord = float2(0, 0);
    v[1].vPosition = p1;
    v[1].vTexcoord = float2(1, 0);
    v[2].vPosition = p2;
    v[2].vTexcoord = float2(1, 1);
    v[3].vPosition = p3;
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

struct PS_UI
{
    vector vUIText : SV_TARGET0;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
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

struct PS_CLOUD_OUT
{
    vector vFinalBuffer : SV_TARGET0;
};

PS_CLOUD_OUT PS_CLOUD(PS_IN In)
{
    PS_CLOUD_OUT Out;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vDepthDesc.x <0.2f)
        discard;
    float fViewZ = vDepthDesc.y * zFar; /*월드 파*/
    vector vWorldPos;
    
     /*클립 스페이스로 변환*/
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    /*z나누기 되돌림*/
    vWorldPos = vWorldPos * fViewZ;
    
    /*공간 변환*/
    vWorldPos = mul(vWorldPos, matProjectionInverse);
    vWorldPos /= vWorldPos.w;
    vWorldPos = mul(vWorldPos, matViewInverse);
    /*구름 카메라로 공간 변환*/
    vWorldPos = mul(vWorldPos, matViewEnvShadow);
    vWorldPos = mul(vWorldPos, matProjEnvShadow);
    
    float2 vTexcoord;
    
    /*다시 z나누기 진행 후에, 0~1공간으로 변환*/
    vTexcoord.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
    vTexcoord.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;
    
    /*같은 공간에 있는 픽셀 추출*/
    vector vCloudDesc = g_CloudTexture.Sample(DefaultSampler, vTexcoord);
    
    // UV 범위 검사 추가
    if (vTexcoord.x < 0 || vTexcoord.x > 1 || vTexcoord.y < 0 || vTexcoord.y > 1)
    {
        discard;

    }

    float4 cloud = g_CloudTexture.Sample(DefaultSampler, vTexcoord);
    float cloudDensity = saturate(cloud.r)*0.2;

    if (cloudDensity < 0.05f)
    {
        discard;
    }
    
    Out.vFinalBuffer = float4(0, 0, 0, cloudDensity); // 초록

   // Out.vFinalBuffer = float4(vTexcoord, 0, 1);
    return Out;
}

struct VS_TexIN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_TexOUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_TexOUT VS_WORLD(VS_TexIN In)
{
    VS_TexOUT Out;
    
    float4 worldPos = mul(float4(In.vPosition, 1.f), transformMatrix);
    float4 viewPos = mul(worldPos, matViewEnvShadow);
    float4 projPos = mul(viewPos, matProjEnvShadow);
    Out.vPosition = projPos;

    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(vector(In.vNormal, 0.f), transformMatrix);
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_CLOUD
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};
struct PS_OUT_CLOUD
{
    vector vColor : SV_TARGET0;
};

PS_OUT_CLOUD PS_CLOUD_RENDER(PS_IN_CLOUD In)
{
    PS_OUT_CLOUD Out;
    float2 uv = float2(In.vTexcoord.x * 0.5, In.vTexcoord.y * 0.5);
    vector vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, uv);
    if (vMtrlDiffuse.a < 0.1)
        discard;
    
    Out.vColor = float4(vMtrlDiffuse.a, vMtrlDiffuse.a, vMtrlDiffuse.a, 1.f);
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

    pass Env_Cloud
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_SCREEN();
        PixelShader = compile ps_5_0 PS_CLOUD();
    }
    pass Render_Cloud
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_WORLD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CLOUD_RENDER();
    }
}

