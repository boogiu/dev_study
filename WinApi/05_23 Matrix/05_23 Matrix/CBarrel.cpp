#include "pch.h"
#include "CBarrel.h"
#include "Components.h"
#include "CKeyManager.h"

CBarrel::CBarrel() :m_fAngle(0.f)
{
}

CBarrel::~CBarrel()
{
}

void CBarrel::Initialize()
{
	CTransform* transform = AddComponent<CTransform>();
	transform->Set_Scale(80.f, 10.f, 0.f);
	transform->Set_Position(30.f, 0.f, 0.f);

	CRigidBody* rigid = AddComponent<CRigidBody>();
	rigid->Set_Friction(true);
	CRenderer* renderer = AddComponent<CRenderer>();
	renderer->Set_Layer(RenderLayer::OBJECT);
}

void CBarrel::Update(float dt)
{
	__super::Update_Component(dt);

	POINT targetPos = CKeyManager::Get_Instance().Get_Cursor();
	D3DXVECTOR3 vTarget = { (float)targetPos.x, (float)targetPos.y, 0.f };

	CTransform* transform = GetComponent<CTransform>();
	CTransform* parent = transform->Get_Parent();
	if (!parent) return;

	// 현재 포신 방향 벡터 
	D3DXVECTOR3 barrelDir = transform->Get_Rectangle().tRB.m_vWorldPos - transform->Get_Rectangle().tLT.m_vWorldPos;
	D3DXVec3Normalize(&barrelDir, &barrelDir);

	// 마우스 방향 벡터 (my 기준)
	D3DXVECTOR3 targetDir = vTarget - transform->Get_Rectangle().tLB.m_vWorldPos;
	D3DXVec3Normalize(&targetDir, &targetDir);

	// 내적 + 외적
	float dot = D3DXVec3Dot(&barrelDir, &targetDir);
	dot = max(-1.f, min(1.f, dot)); // 안정화

	float crossZ = barrelDir.x * targetDir.y - barrelDir.y * targetDir.x;

	float angleRad = acosf(dot);
	if (crossZ < 0) angleRad *= -1.f;

	//누적 방식으로 적용해야 회전이 매끄럽게 유지됨
	m_fAngle += D3DXToDegree(angleRad);

	// 회전 적용
	transform->Set_Pivot(0.f,0.f,0.f); // 부모 기준
	transform->Set_Orbit(0.f, 0.f, m_fAngle);
}



void CBarrel::Late_Update(float dt)
{
	__super::Late_Update_Component(dt);
}

void CBarrel::Release()
{
}
