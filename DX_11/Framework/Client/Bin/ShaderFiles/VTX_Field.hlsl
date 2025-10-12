#include "Shader_Define.hlsl"


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
    vector vWorldPos = mul(vector(In.vPosition, 1.f), matWorld[TransformIndex]);
    
    //빛의 방향의 반대와 월드노멀의 내적을 통해 그 각도를 구해줌 (최소 0을 내려가지 않도록)
    Out.vShade = saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vWorldNormal)), 0.f) + (vLightAmbient * vMtrlAmbient));
    vector vReflect = reflect(normalize(vLightDir), normalize(vWorldNormal));
    vector vLook = vWorldPos - vCamPosition;
    
    Out.fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), fSpecularPow * 100);
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vShade : TEXCOORD1;
    float fSpecular : TEXCOORD2;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
   //빛의 색상 * 빛의 강도 * 텍스처 색깔
  Out.vColor = vLightDiffuse * vMtrlDiffuse * In.vShade +
     (vLightSpecular * vMtrlSpecular) * In.fSpecular;
    
    if(Out.vColor.a < 0.2)
        discard;
    return Out;
}

PS_OUT PS_GRD(PS_IN In)
{
    PS_OUT Out;
    
    vector Diffuse = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    

    if(Diffuse.a < 0.2f)
        discard;
    Out.vColor = Diffuse;
    return Out;
}

PS_OUT PS_Edge(PS_IN In)
{
    PS_OUT Out;
    vector Diffuse = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
   
    if (Diffuse.a < 0.2f)
        discard;
    
    Out.vColor = Diffuse;
;
    return Out;
}


PS_OUT PS_BLEND(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor = vMtrlDiffuse;
    
    Out.vColor.a = 0.4f;
    Out.vColor.b += 0.05f;
    
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
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass Palette
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_GRD();
    }
    pass Edge
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Edge();
    }
    pass ForceBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_BLEND();
    }
}