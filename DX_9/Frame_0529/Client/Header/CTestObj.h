#pragma once
#include "CGameObject.h"

namespace Engine {
	class CTransform;
	class CMeshRenderer;
	class CRigidBody;
}

class CTestObj :
	public Engine::CGameObject
{

private:
	explicit CTestObj();
	virtual ~CTestObj();
public:
	static CTestObj* Create();

public:
	HRESULT Ready_GameObject() override;
	void Update_GameObject(_float&dt) override;
	void LateUpdate_GameObject(_float&dt) override;
private:
	void Key_Check(float dt);
private:
	void Free() override;

private:
	CTransform* m_pTransform;
	CMeshRenderer* m_pRenderer;
	CRigidBody* m_pRigid;
};

