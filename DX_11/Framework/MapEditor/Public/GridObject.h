#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CGridObject final:
    public CGameObject
{
    struct HittedIndex {
        _uint X={};
        _uint Y={};
        _uint Z={};

        _float4 vEdgeMin = {};
        _float4 vEdgeMax = {};
    };
protected:
    CGridObject();
    CGridObject(const CGridObject& rhs);
    virtual ~CGridObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    _float3 Get_HitPos() { return m_HittedPos; };
    void Get_HitIndex(_uint* X, _uint* Y, _uint* Z);
public:
    void Render_GUI() override;

private:
    void Check_Dragging(TILESYSTEM_INFO ContextInfo);
private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };


    SHADER_PARAM* pParam = { nullptr };

    _uint m_iScaleX = {};
    _uint m_iScaleZ = {};

    _float3 m_HittedPos = {};
    _bool isDragging = { false };
    _float3 m_DragPivotPos = {};
    HittedIndex m_HittedIdx = {};

    _uint m_NowYIndex = {};
public:
    static CGridObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END
