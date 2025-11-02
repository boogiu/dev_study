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

	virtual void Pre_EngineUpdate(_float dt)override;
	virtual void Post_EngineUpdate(_float dt)override;

	virtual void Priority_Update(_float dt)override;
	virtual void Update(_float dt)override;
	virtual void Late_Update(_float dt)override;

public :
	_uint Get_Priority() { return m_iPriority; };
	void Set_Priority(_uint priority) { m_iPriority = priority; }
	void Set_CenterPos(_float2 pos) { m_fX = pos.x; m_fY = pos.y; }
	void Set_Size(_float2 size) { m_fSizeX = size.x; m_fSizeY = size.y; }
	_float2 Get_CenterPos() { return{ m_fX ,m_fY }; }

public:
	void Render_GUI() override;

public:
	void Update_UITransform();
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

	
	_float2 Align_To(ANCHOR pivot, _float2 _pivot);

public:
	void Set_OnSystem(const string& Level, _int systemIndex) { m_Level = Level; m_SystemIndex = systemIndex; }
	_int Get_SystemIndex() { return m_SystemIndex; }
	string Get_SystemLevel() { return m_Level; }

protected:
	_float m_WinSizeX = {};
	_float m_WinSizeY = {};

	_float m_fX = {};
	_float m_fY = {};
	_float m_fSizeX = {};
	_float m_fSizeY = {};
	_uint m_iPriority = {UINT_MAX};

	_float m_fRadian = {};

	string m_Level = {};
	_int m_SystemIndex = {-1};
public:
	virtual void Free() override;
};
NS_END
