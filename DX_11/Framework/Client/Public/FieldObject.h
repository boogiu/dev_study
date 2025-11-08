#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldObject :
    public CGameObject
{
protected:
    CFieldObject();
    CFieldObject(const CFieldObject& rhs);
    virtual ~CFieldObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;
public:
    virtual HRESULT Sync_MapData(MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable);

protected:
    _uint m_iObjType = {};
    TILE_INDEX m_SyncedIndex = {};

public:
    void Free() override;
};

NS_END