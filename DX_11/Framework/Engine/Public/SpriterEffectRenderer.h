#pragma once
#include "IEffectRenderer.h"
class CSpriterEffectRenderer :
    public IEffectRenderer
{
    struct SpriteInstanceData
    {
        _float4x4 WorldMatrix;
        _float4  vColor;
        _float4   TexInfo; // (Frame, Cols, Rows, 0)
    };

private:
    CSpriterEffectRenderer();
    ~CSpriterEffectRenderer();

public:
    HRESULT Initialize(ID3D11Device* pDevice);
    void Queue(const EffectSpriteDrawDesc& desc);
    void RenderAll(ID3D11DeviceContext* pContext);  // Pass ºÐ¸® + Instancing + Draw
    void RenderPass(ID3D11DeviceContext* pContext, const string& passName, vector<EffectSpriteDrawDesc>& list);
    size_t DrawBatch(ID3D11DeviceContext* pContext, vector<EffectSpriteDrawDesc>& list, size_t startIndex);

private:
    vector<EffectSpriteDrawDesc> m_DrawCmds;

    _uint m_InstanceCount = {};
    ID3D11Buffer*       m_InstanceBuffer = { nullptr };
    class CVIBuffer*              m_VBQuad = { nullptr };
    class CShader*     m_SpriteEffectShader = { nullptr };

public:
    static CSpriterEffectRenderer* Create(ID3D11Device* pDevice);
    virtual void Free();
};

