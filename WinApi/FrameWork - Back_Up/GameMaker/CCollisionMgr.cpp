#include "pch.h"
#include "CCollisionMgr.h"
#include "CObject.h"
#include "CColider.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"

CCollisionMgr* CCollisionMgr::m_pInstance = nullptr;

CCollisionMgr::CCollisionMgr()
	: m_bRender(false)
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Initialize()
{
}

void CCollisionMgr::Update()
{
	if (CKeyMgr::Get_Instance()->GetKeyTap(VK_F1)) {
		m_bRender = !m_bRender;
	}
	CheckCollisionRectToRect(OBJ_TAG::PLAYER, OBJ_TAG::PLATFORM);
	CheckCollisionRectToRect(OBJ_TAG::PLATFORM, OBJ_TAG::PROJECTILE);

	CheckObjectOnGrounded(PLAYER);

}

void CCollisionMgr::Late_Update()
{
}

void CCollisionMgr::Render(HDC _hDC)
{
	if (m_bRender) {
		RenderCollider(PLATFORM, _hDC);
		RenderCollider(PLAYER, _hDC);
		RenderCollider(PROJECTILE, _hDC);
	}
}

void CCollisionMgr::Release()
{
}


void CCollisionMgr::CheckObjectOnGrounded(OBJ_TAG rightTag)
{
	const auto& ObjVec =CObjMgr::Get_Instance()->Get_ObjectVector(rightTag);
	const auto& PlatformVec = CObjMgr::Get_Instance()->Get_ObjectVector(PLATFORM);

	for (CObject* Obj : ObjVec) {
		CColider* objCol = Obj->Get_Collider();
		if (!objCol || !objCol->isActive()) continue;

		Obj->Set_Grounded(false); // 일단 초기화

		for (CObject* Platform : PlatformVec) {
			CColider* platFormCol = Platform->Get_Collider();
			if (!platFormCol || !platFormCol->isActive()) continue;

			const RECT& objRect = objCol->Get_Rect();
			const RECT& platRect = platFormCol->Get_Rect();
			RECT tmpRect;

			if (IntersectRect(&tmpRect, &objRect, &platRect)) {
				// 아래에서 위로 박히는 모양인지 확인
				float objBottom = (objRect.top + objRect.bottom) * 0.5f;
				float platTop = platRect.top;

				// 아주 약간 아래로 닿아있고, 수직 충돌이면
				if (objBottom <= platTop + 5.f &&
					(tmpRect.right - tmpRect.left > tmpRect.bottom - tmpRect.top)) {
					Obj->Set_Grounded(true);
					break; // 하나라도 닿았으면 grounded
				}
			}
		}
	}
}

void CCollisionMgr::CheckCollisionRectToRect(OBJ_TAG rightTag, OBJ_TAG leftTag)
{
	const auto& rightVec = CObjMgr::Get_Instance()->Get_ObjectVector(rightTag);
	const auto& leftVec = CObjMgr::Get_Instance()->Get_ObjectVector(leftTag);

	for (CObject* rightObj : rightVec) {
		CColider* rightCol = rightObj->Get_Collider();
		if (!rightCol || !rightCol->isActive()) continue;

		for (CObject* leftObj : leftVec) {
			CColider* leftCol = leftObj->Get_Collider();
			if (!leftCol || !leftCol->isActive()) continue;

			RECT tmpRect;
			if (IntersectRect(&tmpRect, &rightCol->Get_Rect(), &leftCol->Get_Rect())) {
				rightObj->OnCollisionEnter(leftObj, tmpRect);
				leftObj->OnCollisionEnter(rightObj, tmpRect);
			}
		}
	}
}

void CCollisionMgr::CheckCollisionLineToRect(OBJ_TAG rightTag, OBJ_TAG leftTag)
{
}

void CCollisionMgr::CheckCollisionCircleToRect(OBJ_TAG rightTag, OBJ_TAG leftTag)
{
}

void CCollisionMgr::RenderCollider(OBJ_TAG tag, HDC _hDC)
{
	const auto& colVec = CObjMgr::Get_Instance()->Get_ObjectVector(tag);

	for (CObject* obj : colVec) {
		CColider* Col = obj->Get_Collider();

		if (Col) {
			
			INFO_EX screenPos = CCamera::Get_Instance()->WorldToScreen(Col->Get_Info());

			RECT col ;
			screenPos.Update_INFO(col);
			Rectangle(_hDC, col.left, col.top, col.right, col.bottom);
		}
	}
}
