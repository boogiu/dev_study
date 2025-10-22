#pragma once
#include "GameObject.h"

NS_BEGIN(Client)
class CFieldStructure :
    public CGameObject
{
public:
    typedef struct tagFieldStructureDesc : public GAMEOBJECT_DESC
    {
        TILE_INDEX Index = {};
        string LevelTag;
        string ModelName;
        string MaterialName;

        tagFieldStructureDesc() DEFAULT;
        virtual ~tagFieldStructureDesc() DEFAULT;
    }FIELDSTR_DESC;

private:
    CFieldStructure();
    CFieldStructure(const CFieldStructure& rhs);
    virtual ~CFieldStructure() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();


public:
    static CFieldStructure* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};

NS_END