#ifndef __SHADER_STATE_HLSL__
#define __SHADER_STATE_HLSL__

RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = Back;
    FrontCounterClockwise = false;
};

RasterizerState RS_Wireframe
{
    FillMode = Wireframe;
    CullMode = NONE;
    FrontCounterClockwise = false;
};

RasterizerState RS_CullFront
{
    FillMode = Solid;
    CullMode = Front;
    FrontCounterClockwise = false;
};


DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = zero;
};

BlendState BS_Default
{
    BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    SrcBlend = Src_Alpha;
    DestBlend = Inv_Src_Alpha; /* µ¤À» »ö»ó*/
    BlendOp = Add;
};

BlendState BS_Blend
{
    BlendEnable[0] = true;
    SrcBlend = one;
    DestBlend = one;
    BlendOp = Add;
};
#endif // __SHADER_STATE_HLSL__