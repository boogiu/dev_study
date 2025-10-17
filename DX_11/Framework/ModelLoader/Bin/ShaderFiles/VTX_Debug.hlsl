#include "Shader_Define.hlsl"

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position; 
};

struct VS_TILEIN
{
    float3 vPosition : POSITION;
    uint iTileOffset : TEXCOORD0;
};

struct VS_TILEOUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    int3 iTileIndex : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    matrix matWV,matWVP;
    matWV = mul(matWorld[TransformIndex], matView);
    matWVP = mul(matWV, matProjection);
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    return Out;
}

VS_TILEOUT VS_TILE(VS_TILEIN In)
{
    VS_TILEOUT Out = (VS_TILEOUT) 0;
    matrix matVP;
    matVP = mul(matView, matProjection);
    Out.vPosition = mul(float4(In.vPosition, 1.f), matVP);
    Out.iTileIndex = uint3(g_TileIndecies[In.iTileOffset].x, g_TileIndecies[In.iTileOffset].y, g_TileIndecies[In.iTileOffset].z);
    Out.vColor = g_TileIndecies[In.iTileOffset].vColor;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_Position; 
};

struct PS_TILEIN
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    int3 TileIndex : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0; 
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out ;
    Out.vColor = float4(1.f,0.f,0.f,0.f);
    return Out;
}

PS_OUT PS_TILE(PS_TILEIN In)
{
    PS_OUT Out;
    Out.vColor = In.vColor;
    return Out;
}


technique11 DefaultTechnique
{
    pass Debug
    {
        SetRasterizerState(RS_Wireframe);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN(); 
    }
    pass DebugTile
    {
        SetRasterizerState(RS_Wireframe);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_TILE();
        PixelShader = compile ps_5_0 PS_TILE();
    }
}