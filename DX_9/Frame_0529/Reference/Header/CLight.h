#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CLight :
	public CComponent
{
private:
	explicit CLight();
	virtual ~CLight() override;

public:
	static CLight* Create(LIGHT_TYPE type);

public:
	HRESULT Ready_Component() override;
	void Update_Component(float& dt) override;
	void LateUpdate_Component(float& dt) override;
	CComponent* Clone() const override;
	static COM_TYPE Get_StaticType() { return COM_TYPE::LIGHT; };
	COM_TYPE Get_Type() override { return Get_StaticType(); };

public:
	void Set_Type(LIGHT_TYPE light);
	void Set_ID(int ID) { m_ID = ID; }
	const _D3DLIGHT9& Get_Light() { return m_tLight; }

	void Set_LightDesc(
		D3DXCOLOR ambient,					// È¯°æ±¤
		D3DXCOLOR diffuse,						// ³­¹Ý»ç±¤
		D3DXCOLOR specular,					// Á¤¹Ý»ç±¤
		float range = 0.f,											// ¶óÀÌÆ® ¹üÀ§
		float att0 = 0.f, float att1 = 0.f, float att2=0.f		// °¨¼è °è¼ö
	);
	void Set_Angle(float theta, float phi);
	void Set_Dir(_vec3 dir);
private:
	bool m_bLight;
	int m_ID;
	_D3DLIGHT9 m_tLight;

private:
	void Free();
};

END