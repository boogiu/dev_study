#include "pch.h"
#include "CPlayer.h"
#include "CInputMgr.h"
#include "Components.h"
#include "CBullet.h"
CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	this->AddComponent(new CTransform);
	this->AddComponent(new CCollider);
	this->AddComponent(new CRigidBody);
	this->GetComponent<CTransform>()->SetPos({ 50.f,50.f });
	this->GetComponent<CTransform>()->SetScale({ 50.f,50.f });
}

void CPlayer::Update()
{
	__super::Update_Component();
	KeyUpdate();

	for (CGameObject* obj : m_list) {
		if (obj) {
			obj->Update();
		}
	}
}

void CPlayer::Late_Update()
{
	MovePlayer();
}

void CPlayer::Render(HDC _hDC)
{
	__super::Render_Component(_hDC);
	for (CGameObject* obj : m_list) {
		if (obj) {
			obj->Render(_hDC);
		}
	}
}

void CPlayer::Release()
{
}

void CPlayer::KeyUpdate()
{
	if (CInputMgr::GetInstance().GetKeyDown(VK_UP)) {
		this->GetComponent<CRigidBody>()->ImPulse({ 0.f,-1.f });
	}
	if (CInputMgr::GetInstance().GetKeyDown(VK_DOWN)) {
		this->GetComponent<CRigidBody>()->ImPulse({ 0.f,1.f });
	}
	if (CInputMgr::GetInstance().GetKeyDown(VK_LEFT)) {
		this->GetComponent<CRigidBody>()->ImPulse({ -1.f,0.f });
	}
	if (CInputMgr::GetInstance().GetKeyDown(VK_RIGHT)) {
		this->GetComponent<CRigidBody>()->ImPulse({ 1.f,0.f });
	}
	if (CInputMgr::GetInstance().GetKeyDown(VK_SPACE)) {
		FireBullet();
	}
}

void CPlayer::MovePlayer()
{

}

void CPlayer::FireBullet()
{
	CBullet* tmp = new CBullet(this->GetComponent<CTransform>()->GetPos());
	tmp->Initialize();
	m_list.push_back(tmp);
}
