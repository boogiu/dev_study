#pragma once

#include "CComponent.h"
BEGIN(Engine)

class CTransform;

class ENGINE_DLL CRenderer :
	public CComponent
{
public:
	enum class RENDERER_TYPE {Mesh, Terrain,SkyBox};

protected:
	explicit CRenderer();
	virtual ~CRenderer() override;

public: 
	static CRenderer* Create(RENDERER_TYPE eType);
public:
	virtual HRESULT Ready_Component() PURE;
	virtual void Render(LPDIRECT3DDEVICE9 pDevice) PURE;
	virtual CComponent* Clone() const PURE;
	void Set_Transform();

public:
	static COM_TYPE Get_StaticType() { return COM_TYPE::RENDERER; }
	COM_TYPE Get_Type() override { return Get_StaticType(); }

protected:
	CTransform* m_pTransform;
};

END