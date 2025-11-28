#include "Shader_Define.hlsl"

uint XScale;
uint ZScale;

float4 vEdgeMin;
float4 vEdgeMax;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPosition : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    float4x4 matWV = mul(ObjectBufferArray[TransformIndex].Transform, matView);
    float4x4 matWVP = mul(matWV, matProjection);
     
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), ObjectBufferArray[TransformIndex].Transform);
    Out.vTexcoord = In.vTexcoord;
   
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPosition : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    float2 newTexcoord = float2(In.vTexcoord.x * XScale / 16, In.vTexcoord.y * ZScale / 16);
    vector GridCol = DiffuseTexture.Sample(PointSampler, newTexcoord);
            
    Out.vColor = GridCol;
    
    float insideX = step(vEdgeMin.x, In.vWorldPosition.x) * step(In.vWorldPosition.x, vEdgeMax.x);  //안에 있음면 1나옴(a보다 b가 작으면 0이됨)
    float insideZ = step(vEdgeMin.z, In.vWorldPosition.z) * step(In.vWorldPosition.z, vEdgeMax.z);
    
    float inside = insideX * insideZ;
    Out.vColor = lerp(GridCol, float4(0.2f, 0.2f, 0.2f, 1), inside);
    Out.vColor.a = 0.2f;
    return Out;
}


technique11 DefaultTechnique
{
    pass Opaque
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}