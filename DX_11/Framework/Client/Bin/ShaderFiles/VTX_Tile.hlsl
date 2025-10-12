#include "Shader_Define.hlsl"

float2 PalettePixel = { 0.5f,0.54f};
int Scale = 100;
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    matrix matWV, matWVP;
    matWV = mul(matWorld[TransformIndex], matView);
    matWVP = mul(matWV, matProjection);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector Diffuse = DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector Mixture = MixtureTexture.Sample(LinearSampler, In.vTexcoord);
    vector Gradation = GradationTexture.Sample(LinearSampler, PalettePixel);
    vector Opacity = OpacityTexture.Sample(LinearSampler, In.vTexcoord);
    
    float hasDiffuse = step(0.01f, Diffuse.a); // 알파 거의 0이면 없음
    float hasGradation = step(0.01f, Gradation.a); // 알파 거의 0이면 없음

    vector BaseColor = Diffuse;

    if (hasDiffuse < 0.2f && hasGradation > 0.2f)
  {
      BaseColor = Gradation;
  }
    else if (hasDiffuse > 0.2 && hasGradation > 0.2f)
  {
      BaseColor = lerp(Diffuse, Gradation, Mixture.a);
  }
    
    else if (hasDiffuse < 0.2f && hasGradation < 0.2f)
    {
        discard;
    }

 // --- 오파시티 적용 ---
 vector Mtrl = (1 - Opacity.a) * BaseColor;
 
 // --- 최종 알파 확인 ---
 if (Mtrl.a < 0.2f)
     discard;

    Out.vColor = BaseColor;
    return Out;
}


PS_OUT PS_BASE(PS_IN In)
{
    PS_OUT Out;
    vector Mask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    vector Mask2 = g_MaskTexture.Sample(LinearSampler, In.vTexcoord * 100);
    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
    vector Palette2 = g_PaletteTexture.Sample(LinearSampler, float2(PalettePixel.x + (1 - Mask.r) * Mask.b, PalettePixel.y));
    vector Opacity = OpacityTexture.Sample(LinearSampler, In.vTexcoord);
   
    vector Grd = (Palette * (1 - Mask2.a) + (Palette2) * (Mask2.a));
    
    Out.vColor = Grd;
    
    return Out;
}


PS_OUT PS_EDGE(PS_IN In)
{
    PS_OUT Out;
    vector Palette = g_PaletteTexture.Sample(DefaultSampler, float2(PalettePixel.x, PalettePixel.y));
    vector Opacity = OpacityTexture.Sample(LinearSampler, In.vTexcoord);

    vector Grd = lerp(float4(0, 0, 0, 0), Palette, Opacity.a);
    if (Grd.a < 0.1f)
        discard;

    Out.vColor = Grd;
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

    pass Base
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
}