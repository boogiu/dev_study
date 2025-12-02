#include "Shader_Define.hlsl"

float fWindTime;
Texture2D IndirectTexture;
Texture2D CloudTexture;
Texture2D CommonTexture;
Texture2D PatternTexture;
float4 skyColor = { 84 / 255.f, 148 / 255.f, 254 / 255.f, 1.f };

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
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    matrix matWV, matWVP;
    
    float3 worldPos = mul(float4(In.vPosition, 1.f), ObjectBufferArray[TransformIndex].Transform).xyz;
    float3 toObj = worldPos - vCamPosition.xyz;
    float dist = dot(toObj, CameraForward);
    float curve = (dist * dist) / PlanetRadius * CurveStrength;
    worldPos.y -= curve;
    
    float4 viewPos = mul(float4(worldPos, 1.f), matView);
    float4 projPos = mul(viewPos, matProjection);

    Out.vPosition = projPos;
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(vector(In.vNormal, 0.f), ObjectBufferArray[TransformIndex].Transform);
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), ObjectBufferArray[TransformIndex].Transform)).xyz;
    Out.vTangent *= -1;
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    // 0~1煎 蘋詭塭 嬪嬴楚 堪檜
    float ndcY = In.vProjPos.y / In.vProjPos.w;
    float h = saturate(ndcY * 0.5f + 0.5f); // -1~1 ⊥ 0~1

    // 掘葷 價Ы (嫩晦虜 餌辨)
    float cloud = CloudTexture.Sample(LinearSampler, In.vTexcoord).r;

    // 式式 雪 つ溯お (瞳渡�� ぅ棚辨) 式式式式式式式式式式式式式
    float3 dayBottom = float3(0.90f, 0.75f, 0.55f); // 雖ゎ摹 薹(擒除 喻嫌/螃溶雖)
    float3 dayMid = float3(0.55f, 0.80f, 1.00f); // 醞除 嫩擎 ж棺儀
    float3 dayTop = float3(0.10f, 0.35f, 0.80f); // 嬪薹 霞и だ嫌

    // 式式 嫘 つ溯お 式式式式式式式式式式式式式式式式式式式式式式式式式式式
    float3 nightBottom = float3(0.05f, 0.07f, 0.12f); // 擒и 紫衛惜 / 陴儀
    float3 nightMid = float3(0.02f, 0.04f, 0.08f);
    float3 nightTop = float3(0.00f, 0.01f, 0.05f); // 剪曖 匐ヰ艇 儀

    // 雪/嫘 爾除
    float day = saturate(skyColor); // 0~1 贗極Щ
    float3 bottom = lerp(nightBottom, dayBottom, day);
    float3 mid = lerp(nightMid, dayMid, day);
    float3 top = lerp(nightTop, dayTop, day);

    // 式式 堪檜縑 評艇 3欽 斜塭等檜暮 式式式式式式式式式式
    float t1 = smoothstep(0.0f, 0.35f, h); // bottom ⊥ mid
    float t2 = smoothstep(0.35f, 0.85f, h); // mid ⊥ top

    float3 col = bottom;
    col = lerp(col, mid, t1);
    col = lerp(col, top, t2);

    // 式式 掘葷/除蕾惜戲煎 髦礎 滲褻 式式式式式式式式式式
    // 掘葷檜 嫩擊熱煙 髦礎 渦 嫩啪
    float cloudIntensity = lerp(0.85f, 1.15f, cloud);
    col *= cloudIntensity;

    // 傘鼠 婁п雖雖 彊啪 驛 и 廓 揚楝邀
    col = saturate(col);

    Out.vDiffuse = float4(col, 1.f);
    return Out;
}



technique11 DefaultTechnique
{
    pass Opaque
    {
        SetRasterizerState(RS_NoCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}