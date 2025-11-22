#pragma once
#include "IEffectRenderer.h"
class CSpriterEffectRenderer :
    public IEffectRenderer
{
    struct SpriteInstanceData
    {
        _float4x4 WorldMatrix;
        _float4  vColor;
        _float4   TexInfo;
    };

private:
    CSpriterEffectRenderer();
    ~CSpriterEffectRenderer();

public:
    HRESULT Initialize(ID3D11Device* pDevice);
    void Queue(const EffectSpriteDrawDesc& desc);
    void RenderAll();  // Pass ºÐ¸® + Instancing + Draw

private:
    unordered_map<string, vector<EffectSpriteDrawDesc>> m_Queues;

    ID3D11Buffer*       m_InstanceBuffer = { nullptr };
    class CVIBuffer*              m_VBQuad = { nullptr };
    class CShader*     m_SpriteEffectShader = { nullptr };

public:
    static CSpriterEffectRenderer* Create(ID3D11Device* pDevice);
    virtual void Free();
};

