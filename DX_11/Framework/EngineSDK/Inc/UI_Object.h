#pragma once
#include "GameObject.h"
NS_BEGIN(Engine)
class ENGINE_DLL CUI_Object abstract : public CGameObject
{

protected:
	CUI_Object();
	CUI_Object(const CUI_Object& rhs);
	virtual ~CUI_Object() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(INIT_DESC* pArg = nullptr)override;
	void Post_EngineUpdate(_float dt)override;
	virtual void Priority_Update(_float dt)override;
	virtual void Update(_float dt)override;
	virtual void Late_Update(_float dt)override;

public:
	void Update_UITransform();
	/*Rotate*/
	void Rotate_Left(_float _radian);

	/*Get Size*/
	_float HalfX() { return m_fSizeX * 0.5f; }
	_float HalfY() { return m_fSizeY * 0.5f; }

	/*Get Anchor*/
	_float2 LT() { return { m_fX - HalfX(), m_fY - HalfY() }; }
	_float2 LC() { return { m_fX - HalfX(), m_fY  }; }
	_float2 LB() { return { m_fX - HalfX(), m_fY + HalfY() }; }

	_float2 CT() { return { m_fX , m_fY - HalfY() }; }
	_float2 Center() { return {m_fX,m_fY}; }
	_float2 CB() { return { m_fX , m_fY + HalfY() }; }

	_float2 RT() { return { m_fX + HalfX(), m_fY - HalfY() }; }
	_float2 RC() { return { m_fX + HalfX(), m_fY }; }
	_float2 RB() { return   {m_fX + HalfX(), m_fY + HalfY()}; }

	
	_float2 Align_To(UI_ANCHOR pivot, _float2 _pivot);

protected:
	_float m_WinSizeX = {};
	_float m_WinSizeY = {};

	_float m_fX = {};
	_float m_fY = {};
	_float m_fSizeX = {};
	_float m_fSizeY = {};

	_float m_fRadian = {};

	_float4x4 m_ViewMat = {};
	_float4x4 m_ProjMat = {};

};
NS_END
