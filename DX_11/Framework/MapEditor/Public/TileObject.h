#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CTileObject :
    public CGameObject
{
private:
    CTileObject();
    CTileObject(const CTileObject& rhs);
    virtual ~CTileObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    void Object_OnGrid(_uint x=0, _uint y = 0, _uint z =0);
    void Set_Selected(_bool selected);

public:
    void Render_GUI() override;

public:
    static CTileObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END