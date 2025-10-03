#pragma once
#include "Component.h"
class CSprite2D final:
    public CComponent
{
private:
    CSprite2D();
    CSprite2D(const CSprite2D& rhs);
    virtual ~CSprite2D();
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
    void Render_GUI();

private:
    class  CVI_Rect* m_pPlane;
    vector<class CTexture*> m_pTextures;

public:
    static CSprite2D* Create();
    virtual CComponent* Clone() override;
    virtual void Free() override;
};

