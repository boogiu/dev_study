#pragma once
#include "Component.h"
NS_BEGIN(Engine)

class ENGINE_DLL CTileBlock :
    public CComponent
{
private:
    CTileBlock();
    CTileBlock(const CTileBlock& rhs);
    virtual ~CTileBlock() DEFAULT;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    void Set_Index(_uint Index);
    void UpdatePosition(const TILESYSTEM_INFO& tileInfo);
    void Set_TilePostion(_uint x, _uint y, _uint z);
    void Get_TilePostion(_uint* x, _uint* y, _uint* z);
    vector<CTileBlock*> Get_Neighbor();

private:
    void CheckTileSystem(); //타일 시스템의 생성 타이밍을 조절해야 함

private:
    _int m_Index = {-1};
    class CTransform* m_pTransform = { nullptr };
    class ITileService* m_pTileSystem = { nullptr };
    class CTexture* m_pPaletteTexture = { nullptr };
    class CTexture* m_pEdgeTexture = { nullptr };

public:
    static CTileBlock* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;

};

NS_END