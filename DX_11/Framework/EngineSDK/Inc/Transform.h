#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CTransform final :
	public CComponent
{

private:
	CTransform();
	CTransform(const CTransform& rhs);
	~CTransform() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;

private:
	_float4x4 m_WorldMatrix = {};
	_float m_fSpeedPerSec;
	_float m_fRotatePerSec;

public:
	static CTransform* Create();
	virtual CComponent* Clone();
	void Free() override;
};
NS_END
