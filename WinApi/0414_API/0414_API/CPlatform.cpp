#include "pch.h"
#include "CPlatform.h"
#include "Components.h"
CPlatform::CPlatform()
{()
}

CPlatform::~CPlatform()
{
	Release();
}

void CPlatform::Initialize()
{
	this->AddComponent(new CTransform);
	this->GetComponent<CTransform>()->SetPos({ WINCX >> 1, WINCY });
	this->GetComponent<CTransform>()->SetScale({ WINCX, 100});

	this->AddComponent(new CCollider);
	VECTOR2 myScale = this->GetComponent<CTransform>()->GetScale();
	this->GetComponent<CCollider>()->SetScale(myScale);
}

void CPlatform::Update()
{
	__super::Update();
}

void CPlatform::Late_Update()
{
	__super::Late_Update();
}

void CPlatform::Render(HDC _hDC)
{
	__super::Render(_hDC);
}

void CPlatform::Release()
{
}
