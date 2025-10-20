
#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
 class CClothParts :
    public CGameObject
{
public:
    typedef struct tagClothDesc : GAMEOBJECT_DESC {
        CGameObject* pPlayer = { nullptr };
        string ClothType = {  };
    }CLOTHES_DESC;

private:
    CClothParts();
    CClothParts(const CClothParts& rhs);
    virtual ~CClothParts() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

public:
    static CClothParts* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END