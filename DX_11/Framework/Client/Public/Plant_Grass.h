#pragma once
#include "FieldObject.h"

NS_BEGIN(Client)
class CPlant_Grass :
    public CFieldObject
{
private:
    CPlant_Grass();
    CPlant_Grass(const CPlant_Grass& rhs);
    virtual ~CPlant_Grass() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    virtual HRESULT Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;

private:
    TILE_INDEX m_Index = {};
public:
    static CPlant_Grass* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END