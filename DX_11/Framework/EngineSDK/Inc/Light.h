#pragma once
#include "Component.h"
NS_BEGIN(Engine)
class CLight :
	public CComponent
{


private:
	CLight();
	CLight(const CLight& rhs);
	virtual ~CLight() DEFAULT;
public:
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(COMPONENT_DESC* pArg) override;
public:
	void Render_GUI();
	void Set_Desc(const LIGHT_DESC& desc);
private:
	LIGHT_DESC m_Light = {}; 

public:
	static CLight* Create(const LIGHT_DESC& desc);
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

NS_END