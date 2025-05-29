#include "pch.h"
#include "CPlayer.h"
#include "Components.h"
#include "CKeyManager.h"
#include "CObjectManager.h"
#include "CBarrel.h"
CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	CTransform* transform = AddComponent<CTransform>();
	transform->Set_Scale(100.f,100.f,0.f);
	transform->Set_Position(0.f,0.f,0.f);
	transform->Set_Rotation(0.f, 0.f,0.f);
	CRigidBody* rigid = AddComponent<CRigidBody>();
	rigid->Set_Friction(true);
	CRenderer* renderer = AddComponent<CRenderer>();
	m_fSpeed = 30.f;
	m_Weapon = CObjectManager::Get_Instance().Create_Object<CBarrel>();
	m_Weapon->Initialize();
	m_Weapon->GetComponent<CTransform>()->Set_Parent(transform);

	m_vNowDir = { 1.f,0.f,0.f };
}

void CPlayer::Update(float dt)
{
	__super::Update_Component(dt);
	Key_Check();
	Rotate_Dir(dt);
}

void CPlayer::Late_Update(float dt)
{
	__super::Late_Update_Component(dt);
}


void CPlayer::Release()
{
}

void CPlayer::Key_Check()
{
	CRigidBody* rigid = GetComponent<CRigidBody>();
	CTransform* transform = GetComponent<CTransform>();

	if (CKeyManager::Get_Instance().Key_Down(VK_LEFT)) {
		rigid->Impulse(-1.f * m_fSpeed, 0.f, 0.f);
		m_vDirectionTo.x = -1;
	}
	if (CKeyManager::Get_Instance().Key_Down(VK_RIGHT)) {
		rigid->Impulse(1.f * m_fSpeed, 0.f, 0.f);
		m_vDirectionTo.x = 1;
	}
	if (CKeyManager::Get_Instance().Key_Down(VK_UP)) {
		rigid->Impulse(0.f, -1.f * m_fSpeed, 0.f);
		m_vDirectionTo.y = -1;
	}
	if (CKeyManager::Get_Instance().Key_Down(VK_DOWN)) {
		rigid->Impulse(0.f, 1.f * m_fSpeed, 0.f);
		m_vDirectionTo.y = 1;
	}
}
void CPlayer::Rotate_Dir(float dt)
{
	CTransform* transform = GetComponent<CTransform>();
	float nowAngle = transform->Get_Rotation().z;
	//현재 각도 기반 벡터 구함
	D3DXVECTOR3 nowDir = {
		cosf(D3DXToRadian(nowAngle)),
		sinf(D3DXToRadian(nowAngle)),
		0.f
	};

	if (m_vDirectionTo.x == 0.f && m_vDirectionTo.y == 0.f)
		return;

	//둘다 정규화
	D3DXVec3Normalize(&nowDir, &nowDir);
	D3DXVec3Normalize(&m_vDirectionTo, &m_vDirectionTo);

	//내적
	float dot = D3DXVec3Dot(&nowDir, &m_vDirectionTo);
	dot = max(-1.f, min(1.f, dot)); // 안정화
	//각도 구함
	float angleRad = acosf(dot);
	//외적으로 방향 확인
	float crossZ = nowDir.x * m_vDirectionTo.y - nowDir.y * m_vDirectionTo.x;
	if (crossZ < 0) angleRad *= -1.f;

	float angleDeg = D3DXToDegree(angleRad);

	// 회전 
	if (fabs(angleDeg) < 0.005f) 
		return;

	// 속도 제한
	float rotateSpeed = 360.f * dt; // 초당 360도 회전 가능
	float deltaAngle = std::clamp(angleDeg, -rotateSpeed, rotateSpeed);

	nowAngle += deltaAngle;
	// 회전 적용
	GetComponent<CTransform>()->Set_Rotation(0.f, 0.f, nowAngle);
}
