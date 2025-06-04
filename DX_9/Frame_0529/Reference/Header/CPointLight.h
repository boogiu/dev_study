#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CPointLight :
	public CComponent
{
private:
	explicit CPointLight();
	virtual ~CPointLight() override;
public:
	static CPointLight* Create();
public:
	HRESULT Ready_Component() override;
	void Update_Component(float dt) override;
	void LateUpdate_Component(float dt) override;
	CComponent* Clone() const override;
	static COM_TYPE Get_StaticType() { return COM_TYPE::POINT_LIGHT; };
	COM_TYPE Get_Type() override { return Get_StaticType(); };

private:
	bool m_bLight;
	_D3DLIGHT9 m_tLight;

private:
	void Free();
};

END