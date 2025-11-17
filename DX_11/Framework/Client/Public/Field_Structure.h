#pragma once
#include "FieldObject.h"

NS_BEGIN(Client)
class CField_Structure :
    public CFieldObject
{
private:
    CField_Structure();
    CField_Structure(const CField_Structure& rhs);
    virtual ~CField_Structure() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    void OnCollisionEnter(COLLISION_CONTEXT ctx) override;
public:
    virtual HRESULT Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable) override;

private:
    TILE_INDEX m_Index;
public:
    static CField_Structure* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END