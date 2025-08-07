#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCollider :
    public CComponent
{
protected:
	explicit CCollider();
	virtual ~CCollider() override;

public:
	HRESULT Ready_Component() PURE;
	void Update_Component(float& dt) PURE;
	void LateUpdate_Component(float& dt) PURE;
	CComponent* Clone() const PURE;

public: 
	void Set_Layer(COLLIDER_LAYER layer) { m_eColType = layer; }
	COLLIDER_LAYER Get_Layer() { return m_eColType ; }

protected:
	void Register_System();
protected:
	COLLIDER_LAYER m_eColType;
};

END