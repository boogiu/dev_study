#pragma once

#include "CComponent.h"
BEGIN(Engine)

class CTransform;
class CStateCache;

class ENGINE_DLL CRenderer :
	public CComponent
{
public:
	enum class RENDERER_TYPE {Mesh, Terrain,SkyBox};

protected:
	explicit CRenderer();
	virtual ~CRenderer() override;

public: 
public:
	virtual HRESULT Ready_Component() PURE;
	virtual void Render(LPDIRECT3DDEVICE9 pDevice) PURE;
	virtual CComponent* Clone() const PURE;
	virtual RENDER_PASS Get_RenderPass() PURE;
	void Set_Transform();

protected:
	CTransform* m_pTransform;
	CStateCache* m_pCache;
};

END