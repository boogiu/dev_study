#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class CAnimator3D;
NS_END

NS_BEGIN(Loader)
class CModelObject :
    public Engine::CGameObject
{
protected:
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

protected:
   virtual virtual HRESULT Load_AIScene(const string& filePath);
   virtual virtual HRESULT Load_Static(const string& fileName);
   virtual virtual HRESULT Load_Animated(const string& fileName);
   virtual virtual HRESULT Save_AIScene();

    _bool HasBones();
    void ReleasPrevModel();

public:
    void Add_Part();

protected:
    _bool IsAnimateModel = { false };
    Importer		m_Importer = {};
    const aiScene* m_pAIScene = { nullptr };
    CAnimator3D* m_pAnimator = { nullptr };

    _float m_fMeshAngle = {};
public:
    static CModelObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();
};

NS_END