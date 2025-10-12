#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)

class CFieldOutBlocks :
    public CGameObject
{
private:
    CFieldOutBlocks();
    CFieldOutBlocks(const CFieldOutBlocks& rhs);
    virtual ~CFieldOutBlocks() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Object_OnGrid(TILE_INDEX index);
    void Set_Selected(_bool selected);
    HRESULT Link_Data(const string& folderName);

public:
    HRESULT Save_Blocks(ofstream& ofs);
public:
    void Render_GUI() override;

private:
    _float2 m_PaletteIndex = {};
    _float2 m_TileScale = {};
    _int m_TileIndex = {};

public:
    static CFieldOutBlocks* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END