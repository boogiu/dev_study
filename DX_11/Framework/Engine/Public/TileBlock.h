#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CTileBlock :
    public CComponent
{
public: 
    enum class Tile_Anchor{NONE,CENTER, LEFTTOP, RIGHTBOTTOM};
private:
    CTileBlock();
    CTileBlock(const CTileBlock& rhs);
    virtual ~CTileBlock() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    TILE_INDEX On_Grid(TILE_INDEX tileIndex);
    void Set_Index(TILE_INDEX tileIndex);
    void Update_Position(const TILESYSTEM_INFO& systemInfo);

private:
    class CTransform* m_pTransform = { nullptr };
    class ITileService* m_pTileSystem = { nullptr };

    TILE_INDEX m_tIndex = {};
public:
    static CTileBlock* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;

};

NS_END