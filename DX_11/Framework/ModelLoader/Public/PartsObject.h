#pragma once
#include "ModelObject.h"
NS_BEGIN(Engine)
class CSkeletalModel;
NS_END
NS_BEGIN(Loader)
class CPartsObject :
	public CModelObject
{
private:
	CPartsObject();
	CPartsObject(const CPartsObject& rhs);
	virtual ~CPartsObject() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt) override;
	void Update(_float dt) override;
	void Late_Update(_float dt) override;
	virtual virtual HRESULT Load_Animated(const string& fileName) override;

public:
	void Inject_Master(class CSkeletalModel* pModel, class CAnimator3D* pAnimator);

public:
	void Render_GUI() override;

private:
	class CSkeletalModel* m_pMasterModel = { nullptr };
	class CAnimator3D* m_pMasterAnimator = { nullptr };
public:
	static CPartsObject* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	virtual void Free();
};
NS_END
