#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CGridObject final:
    public CGameObject
{
    struct HittedArea {
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
    TILE_INDEX Get_HitIndex();
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
    HittedArea m_HittedArea = {};
    TILE_INDEX m_HittedIndex = {};
    TILE_INDEX m_NowIndex = {};
public:
    static CGridObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END
