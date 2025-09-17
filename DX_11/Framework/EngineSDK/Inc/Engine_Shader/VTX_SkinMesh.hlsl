#include "Shader_Define.hlsl"

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
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
    
    matWV = mul(matWorld, matView);
    matWVP = mul(matWV, matProjection);
    
    /*월드 포지션 이전에 본 행렬*/
    float4x4 BoneMatrix =
    (   BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
         BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
         BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
         BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w);
    
     //벡터를 본 변환해줌
    vector BonePos = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector BoneNoraml = mul(vector(In.vNormal, 0.f), BoneMatrix);
    
    Out.vTexcoord = In.vTexcoord;
    Out.vPosition = mul(BonePos, matWVP);
    
    //벡터를 월드 변환해줌 -> 빛 연산을 위해 뷰프로젝션은 안함
    vector vWorldPos = mul(BonePos, matWorld);
    vector vWorldNormal = mul(BoneNoraml, matWorld);
    
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
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord );
    
   //빛의 색상 * 빛의 강도 * 텍스처 색깔
   Out.vColor = vLightDiffuse * vMtrlDiffuse * In.vShade +
       (vLightSpecular * vMtrlSpecular) * In.fSpecular;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Opaque
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }  
}

