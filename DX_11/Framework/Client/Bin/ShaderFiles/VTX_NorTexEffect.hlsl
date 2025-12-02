#include "Shader_Define.hlsl"
float fElapsedTime;
float fRadian;
float2 fAtlasIndex;
float2 fAtlasScale;
Texture2D DepthTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};


VS_OUT VS_BILLBOARD(VS_IN In)
{
    VS_OUT Out;
    matrix matWV, matWVP;
    
    float3 objectWorld = mul(float4(0, 0, 0, 1), ObjectBufferArray[TransformIndex].Transform).xyz;
    float3 toObj = objectWorld - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    objectWorld.y -= curve;
    
    float2 worldScale;
    worldScale.x = length(ObjectBufferArray[TransformIndex].Transform._11_12_13);
    worldScale.y = length(ObjectBufferArray[TransformIndex].Transform._21_22_23);
    
    float3 camRight = normalize(matViewInverse._11_12_13);              // X축
    float3 camUp = normalize(matViewInverse._21_22_23);                 // Y축
    float3 camForward = normalize(matViewInverse._31_32_33);    // Z축
    
    float c = cos(fRadian);
    float s = sin(fRadian);
    float3 right = camRight * c + camUp * s;
    float3 up = camUp * c - camRight * s;
    
    // 정점 위치(로컬 quad 좌표) 적용
    float3 worldPos = objectWorld
                                     + right * In.vPosition.x * worldScale.x
                                    + up * In.vPosition.y * worldScale.y;

    float4 vPos = mul(float4(worldPos, 1.f), matView);
    Out.vPosition = mul(vPos, matProjection);

    // 텍스처 좌표
    Out.vTexcoord = In.vTexcoord;

    // Normal은 카메라를 향하도록 고정 (조명 넣는다면 기존 메시 normals 불필요)
    Out.vNormal = normalize(-camForward);

    // tangent/bitangent도 카메라 기준 (필요할 때만)
    Out.vTangent = camRight;
    Out.vBinormal = camUp;

    Out.vProjPos = Out.vPosition;
    return Out;
}

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    matrix matWV = mul(ObjectBufferArray[TransformIndex].Transform, matView);
    matrix matWVP = mul(matWV, matProjection);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    float3 worldNormal = mul(float4(In.vNormal, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    Out.vNormal = normalize(worldNormal);

    Out.vProjPos = Out.vPosition;

    float3 worldTangent = mul(float4(1.f, 0.f, 0.f, 0.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    Out.vTangent = normalize(worldTangent * -1);

    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_DUST(PS_IN In)
{
    PS_OUT Out;

    float4 vMtrlDiffuse = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 vMixture = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMixture.r < fElapsedTime)
      discard;
        
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.xyz = 1.f;
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);

    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

PS_OUT PS_CRAFT(PS_IN In)
{
    PS_OUT Out;
    float2 uv = In.vTexcoord * fAtlasScale + fAtlasIndex * fAtlasScale;
    
    float4 vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, uv);

    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.xyz = 1.f;
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

PS_OUT PS_PARTICLE(PS_IN In)
{
    PS_OUT Out;
    
    float4 vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.xyz = 1.f;
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}


PS_OUT PS_WATERPONG(PS_IN In)
{
    PS_OUT Out;
    
    float4 vMtrlDiffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.r < fElapsedTime+0.1f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

PS_OUT PS_FRAMEDUST(PS_IN In)
{
    PS_OUT Out;
    float2 uv = In.vTexcoord * fAtlasScale + fAtlasIndex;
 
    float4 vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, uv);
    float4 vMixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vMixture.r < fElapsedTime)
        discard;
    
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.xyz = 1.f;
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);
    Out.vDiffuse = vMtrlDiffuse;
    
    return Out;
}

PS_OUT PS_SMOKE(PS_IN In)
{
    PS_OUT Out;

    float4 vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 vMixture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    
    //if (vMixture.a < fElapsedTime-0.2f)
    //    discard;
        
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.xyz = 1.f;
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);
    
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}


PS_OUT PS_EMOTION(PS_IN In)
{
    PS_OUT Out;
    float2 uv = In.vTexcoord * fAtlasScale + fAtlasIndex * fAtlasScale;

    float4 vMtrlDiffuse = DiffuseTexture.Sample(DefaultSampler, uv);
    float4 vMaskTexture = MixtureTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    if (vMaskTexture.a < fElapsedTime*0.5f-0.2f)
        discard;
    
    float2 vTexcoord;
    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vOldDepthDesc = DepthTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * zFar;
 
    vMtrlDiffuse.a *= saturate(fOldViewZ - In.vProjPos.w);
    
    vector vNormalDesc = NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
 
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zFar, 0.f, 1.f);
    return Out;
}


struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out;

    float3 worldPos = mul(float4(In.vPosition, 1.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(float4(worldPos, 1.f), matShadowView);
    float4 projPos = mul(viewPos, matShadowProjection);
    
    Out.vPosition = projPos;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN_SHDOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    vector vShadow : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHDOW In)
{
    PS_OUT_SHADOW Out;
 
    Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / zShadowFar, 0.f, 0.f);
   
    return Out;
}

technique11 DefaultTechnique
{
    pass Dust
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DUST();
    }
    pass Craft
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CRAFT();
    }

    pass Particle
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PARTICLE();
    }
    pass WaterPong
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_WATERPONG();
    }
    pass FrameDust
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FRAMEDUST();
    }
    pass Smoke
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_ReadOnly, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SMOKE();
    }

    pass Emotion
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_BILLBOARD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_EMOTION();
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
}
