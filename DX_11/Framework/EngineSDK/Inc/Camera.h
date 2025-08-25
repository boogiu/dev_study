#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class ENGINE_DLL CCamera final :
    public CComponent
{
private:
	CCamera();
	CCamera(const CCamera& rhs);
	~CCamera() DEFAULT;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(COMPONENT_DESC* pArg) override;

private:
	class CTransform* m_pTransform = { nullptr };

public:
	static CCamera* Create();
	virtual CComponent* Clone();
	void Free() override;
};
NS_END
