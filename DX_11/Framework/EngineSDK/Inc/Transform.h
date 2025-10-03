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
	HRESULT Initialize(COMPONENT_DESC* pArg) override;

public:
	void Translate(_fvector momentVector);
	void Rotation(_fvector eulerVector);
	void Rotation(_fvector vAxis, _float fRadian);
	void AddScale(_fvector scale);

	void Set_Pos(const _float3& position);
	void Rotate(const _float3& eular);
	void Scale(const _float3& scale);

	void LookAt(_fvector vAt);

public:
	_vector Get_Pos() { return XMLoadFloat4(&m_vPosition); };
	_vector Get_QuaternionRotate() { return XMLoadFloat4(&m_qRotation); };
	_vector Get_Scale() { return XMLoadFloat4(&m_vScale); };
	 _vector Dir(STATE eState);

	 _float4x4* Get_WorldMatrix();
	 _float4x4 Get_InverseWorldMatrix();
	 _float4x4* Get_InverseWorldMatrix_Ptr();

public:
	void Set_ParentTransform(CTransform* pParentTransform);
	void TranslateMatrix(_fmatrix matrix);
public:
	virtual void Render_GUI() override;

private:
	void Update_Transform();

private:
	_bool m_bDirty = {};

	_float4 m_vPosition = {};
	_float4 m_vScale = { 1.f,1.f, 1.f, 0.f };
	_float4 m_qRotation = {};	

	_float4x4 m_WorldMatrix = {};
	CTransform*  m_pParentTransform= { nullptr };

	_float4x4 m_LocalMatrix = {};
	_float4x4 m_WorldInversMatrix = {};

public:
	static CTransform* Create();
	virtual CComponent* Clone();
	void Free() override;
};
NS_END
