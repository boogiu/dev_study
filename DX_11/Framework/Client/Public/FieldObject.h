#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldObject :
    public CGameObject
{
public:
    typedef struct tagFieldObjectDesc : public GAMEOBJECT_DESC
    {
        TILE_INDEX Index = {};
        string LevelTag;
        string ModelName;
        string MaterialName;
    }FIELDOBJ_DESC;

private:
    CFieldObject();
    CFieldObject(const CFieldObject& rhs);
    virtual ~CFieldObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI()override;

public:
    static CFieldObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END