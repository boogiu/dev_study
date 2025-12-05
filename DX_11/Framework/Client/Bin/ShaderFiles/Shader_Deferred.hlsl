#include "Shader_Define.hlsl"

matrix g_WorldMatrix;

Texture2D g_NormalTexture;
Texture2D g_DiffuseTexture;
Texture2D g_ShadeTexture;
Texture2D g_SpecularTexture;
Texture2D g_EmmisiveTexture;
Texture2D g_DepthTexture;
Texture2D g_ShadowTexture;

Texture2D g_FinalTexture;
Texture2D g_UITexture;
Texture2D g_PostProcessTexture;

vector g_vLightDir;
vector g_vLightPos;
float      g_fLightRange;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;
vector g_vMtrlAmbient = 1.f;
vector g_vMtrlSpecular = 1.f;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    matWVP = mul(g_WorldMatrix, matOrthograph);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT_BACKBUFFER
{
    vector vBackBuffer : SV_TARGET0;
};


struct PS_OUT_LIGHT
{
    vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

float contrast = 1.2f;
PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out;

    // ── Normal decode ─────────────────────────────
    float4 vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 N = normalize(vNormalDesc.xyz * 2.f - 1.f); // 0~1 → -1~1

    // 필요하면 여기서만 뒤집기
    N *= -1.0f;

    // ── Directional Light 방향 ────────────────────
    float3 L = normalize(-g_vLightDir.xyz); // 라이트 → 픽셀

    float ndl = saturate(dot(N, L)); // 기본 램버트

    ndl = saturate((ndl - 0.5f) * contrast + 0.5f);

    // 너무 한쪽만 밝지 않게 바닥 올리기
    float minLambert = 0.3f; // 0.3~0.6 사이에서 취향껏 조절
    float lambert = lerp(minLambert, 1.0f, ndl);

    float3 diffuse = g_vLightDiffuse.rgb * lambert;
    float3 ambient = g_vLightAmbient.rgb * g_vMtrlAmbient.rgb;

    Out.vShade.rgb = diffuse + ambient;
    Out.vShade.a = 1.0f;

    // ── Depth → WorldPos 복원 (스펙큘러용) ─────────
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);

    float4 vPosNDC;
    vPosNDC.x = In.vTexcoord.x * 2.f - 1.f;
    vPosNDC.y = In.vTexcoord.y * -2.f + 1.f;
    vPosNDC.z = vDepthDesc.x;
    vPosNDC.w = 1.f;

    float4 vViewPos = mul(vPosNDC, matProjectionInverse);
    vViewPos /= vViewPos.w;
    float4 vWorldPos = mul(vViewPos, matViewInverse);

    // ── Specular ──────────────────────────────────
    float3 V = normalize(vCamPosition.xyz - vWorldPos.xyz);
    float3 R = reflect(-L, N);

    float spec = pow(saturate(dot(V, R)), 25.0f); // 샤이니 정도
    Out.vSpecular.rgb = g_vLightSpecular.rgb * g_vMtrlSpecular.rgb * spec;
    Out.vSpecular.a = 1.0f;

    return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out;

    // ── Normal decode ─────────────────────────────
    float4 vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 N = normalize(vNormalDesc.xyz * 2.f - 1.f); // -1~1
    // 필요하면 여기서만 뒤집기
    N *= -1.0f;

    // ── Depth → WorldPos 복원 ─────────────────────
    float4 vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);

    float4 vPosNDC;
    vPosNDC.x = In.vTexcoord.x * 2.f - 1.f;
    vPosNDC.y = In.vTexcoord.y * -2.f + 1.f;
    vPosNDC.z = vDepthDesc.x;
    vPosNDC.w = 1.f;

    float4 vViewPos = mul(vPosNDC, matProjectionInverse);
    vViewPos /= vViewPos.w;
    float4 vWorldPos = mul(vViewPos, matViewInverse);

    // ── Point Light 방향/감쇠 ─────────────────────
    
    float3 lightPosCurved = ApplyCurve(g_vLightPos.xyz); // 라이트도 곡률 적용
    float3 Lvec = (lightPosCurved.xyz - vWorldPos.xyz); // 라이트 → 픽셀
    float dist = length(Lvec);
    float3 L = Lvec / max(dist, 1e-4f);

    // 거리 감쇠 : 중심은 밝고, 바깥으로 부드럽게 떨어지게
    float fAtt = saturate(1.0f - dist / g_fLightRange);
    fAtt = fAtt * fAtt; // 한 번 더 제곱해서 중심 쪽 강조 (필요 없으면 지워도 됨)

    // ── 부드러운 램버트 ───────────────────────────
    float ndl = saturate(dot(N, L)); // 0~1

    // 너무 한쪽만 밝아지지 않게 바닥 올려줌
    // ndl 0~1  →  0.6~1.0 정도로 압축
    float minLambert = 0.6f; // 원 안이 거의 균일해지는 정도
    float lambert = lerp(minLambert, 1.0f, ndl);

    float3 diffuse = g_vLightDiffuse.rgb * lambert * fAtt;
    float3 ambient = g_vLightAmbient.rgb * g_vMtrlAmbient.rgb * fAtt;

    Out.vShade.rgb = diffuse + ambient;
    Out.vShade.a = 1.0f;

    // ── 스펙큘러 (있어도 되고, 너무 튀면 빼도 됨) ─────────
    float3 V = normalize(vCamPosition.xyz - vWorldPos.xyz);
    float3 R = reflect(-L, N);

    float spec = pow(saturate(dot(V, R)), 50.0f);
    Out.vSpecular.rgb = g_vLightSpecular.rgb * g_vMtrlSpecular.rgb * spec * fAtt;
    Out.vSpecular.a = 1.0f;

    return Out;
}

PS_OUT_BACKBUFFER PS_MAIN_COMBINED(PS_IN In)
{
    PS_OUT_BACKBUFFER Out;
    
    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vDiffuse.a == 0.0f)
        discard;
    
    vector vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vEmmision = g_EmmisiveTexture.Sample(DefaultSampler, In.vTexcoord);

    // 1) 조명만 먼저 계산
    vector vLighting = vDiffuse * vShade + vSpecular;

    // ====== 깊이 / 그림자 계산 (기존 그대로) ======
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * zFar;

    vector vWorldPos;
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, matProjectionInverse);
    vWorldPos = mul(vWorldPos, matViewInverse);
    vWorldPos = mul(vWorldPos, matShadowView);
    vWorldPos = mul(vWorldPos, matShadowProjection);
    
    float2 vShadowTexcoord;
    vShadowTexcoord.x = vWorldPos.x / vWorldPos.w * 0.5f + 0.5f;
    vShadowTexcoord.y = vWorldPos.y / vWorldPos.w * -0.5f + 0.5f;
    
    float4 vLightDepthDesc = g_ShadowTexture.Sample(DefaultSampler, vShadowTexcoord);

    if (vWorldPos.w - 0.1f > vLightDepthDesc.y * zShadowFar)
    {
        // 2) 그림자는 조명에만 적용
        vLighting *= 0.8f;
    }

    // 3) 최종 색 = 조명 + 에미션
    Out.vBackBuffer = float4(vLighting.rgb + vEmmision.rgb, 1.f);

    return Out;
}

float BloomThreshold = 1.0;
PS_OUT_BACKBUFFER PS_MAIN_BRIGHTNESS(PS_IN In)
{
    PS_OUT_BACKBUFFER Out;

    vector vScene = g_FinalTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vector vEmissive = g_EmmisiveTexture.Sample(DefaultSampler, In.vTexcoord);

    vector vBrightFromScene = max(vScene - BloomThreshold, 0.0f);

    vector vBrightFromEmissive = vEmissive;

    vector vBloomSrc = vBrightFromScene + vBrightFromEmissive;

    Out.vBackBuffer = float4(vBloomSrc.rgb, 1.0f);
    return Out;
}

float exposure = 2.3f; // 살짝 밝게
float whitePoint = 8.f; // 하이라이트 빨리 눌러서 부드럽게.8;

float ReinhardWhitePreserve(float L, float Lwhite)
{
    float Lwhite2 = Lwhite * Lwhite;
    float numerator = L * (1.0 + L / Lwhite2);
    float denominator = 1.0 + L;
    return numerator / denominator;
}

float3 ToneMapReinhardWhite(float3 hdrColor)
{
    float3 exposed = hdrColor * exposure;
    float L = dot(exposed, float3(0.2126, 0.7152, 0.0722));
    float L_mapped = ReinhardWhitePreserve(L, whitePoint); // 4.f, 6.f, 8.f, 11.2f 
    float scale = (L > 0.0f) ? (L_mapped / L) : 0.0f;
    return exposed * scale;
}

float4 PS_MAIN_FINAL(PS_IN In) : SV_Target
{
    float4 scene = g_FinalTexture.Sample(DefaultSampler, In.vTexcoord);
    float4 ui = g_UITexture.Sample(DefaultSampler, In.vTexcoord);
    float4 postProcess = g_PostProcessTexture.Sample(DefaultSampler, In.vTexcoord);
   
    float3 hdr = g_FinalTexture.SampleLevel(LinearSampler, In.vTexcoord, 0).rgb;
    float3 mapped = ToneMapReinhardWhite(hdr);

    return float4((1 - ui.a) * mapped.xyz + (ui.a * ui.rgb), 1.f);
}


//float4 PS_MAIN_FINAL(PS_IN In) : SV_Target
//{
//    float4 scene = g_FinalTexture.Sample(DefaultSampler, In.vTexcoord);
//    float4 ui = g_UITexture.Sample(DefaultSampler, In.vTexcoord);
//    float4 postProcess = g_PostProcessTexture.Sample(DefaultSampler, In.vTexcoord);
//
//    float3 hdr = g_FinalTexture.SampleLevel(LinearSampler, In.vTexcoord, 0).rgb;
//    float3 mapped = 1.0 - exp(-exposure * hdr);
//    return float4(mapped, 1.0);
//    
//    return float4((1 - ui.a) * mapped.xyz + (ui.a * ui.rgb), 1.f);
//    //return float4((1 - ui.a) * scene.xyz + (ui.a * ui.rgb), 1.f);
//}

technique11 DefaultTechnique
{
    pass Directional
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Point
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

    pass Combined
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COMBINED();
    }
    pass Brightness
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BRIGHTNESS();
    }
    pass Final
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FINAL();
    }
}

