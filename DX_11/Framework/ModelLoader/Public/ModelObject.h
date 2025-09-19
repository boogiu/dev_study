#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CAnimator3D;
NS_END
NS_BEGIN(Loader)

class CModelObject :
    public CGameObject
{
private:
    CModelObject();
    CModelObject(const CModelObject& rhs);
  virtual ~CModelObject() DEFAULT;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
public:
   void Render_GUI() override;
private:
    HRESULT Load_AIScene(const string& filePath);
    HRESULT Load_Static(const string& fileName);
    HRESULT Load_Animated(const string& fileName);
    HRESULT Save_AIScene();

    _bool HasBones();

    void RealesPrevModel();
private:
    _bool IsAnimateModel = { false };
    Importer		m_Importer = {};
    const aiScene* m_pAIScene = { nullptr };

    CAnimator3D* m_pAnimator = { nullptr };
public:
    static CModelObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END