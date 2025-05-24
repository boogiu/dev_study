#include "pch.h"
#include "CCollisionMgr.h"
#include "CObject.h"
#include "CCollider.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CAttackBox.h"
#include "CAttackLine.h"
#include "CRader.h"

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

	CheckObjectOnGrounded(PLAYER);
	CheckObjectOnGrounded(MONSTER);
	CheckObjectOnGrounded(PROJECTILE);
	CheckObjectOnGrounded(ITEM);
	CheckObjectOnGrounded(PARTICLE);

	CheckObjectOnWall(PLAYER);
	CheckObjectOnWall(MONSTER);
	CheckObjectOnWall(PROJECTILE);
	CheckObjectOnWall(PARTICLE);

	CheckRectAtkCollision(PLAYER);
	CheckRectAtkCollision(MONSTER);

	CheckLineAtkCollision(PLAYER);
	CheckLineAtkCollision(MONSTER);
}

void CCollisionMgr::Late_Update()
{
	//매 업데이트 이후에 어택 박스 디액티브 된 것은 빼주기.
	for (int i = 0; i < (int)MAX_OBJECT; ++i) {
		if (m_AtkBoxContainer[(OBJ_TAG)i].empty()) continue;
		OBJ_TAG nowTag = (OBJ_TAG)i;
		auto iter = m_AtkBoxContainer[nowTag].begin();
		for (; iter != m_AtkBoxContainer[nowTag].end(); ) {
			if ((*iter)->isActive() == false) {
				iter = m_AtkBoxContainer[nowTag].erase(iter);
			}
			else {
				++iter;
			}
		}
	}

	//라인도 마찬가지
	for (int i = 0; i < (int)MAX_OBJECT; ++i) {
		if (m_AtkLineContainer[(OBJ_TAG)i].empty()) continue;
		OBJ_TAG nowTag = (OBJ_TAG)i;
		auto iter = m_AtkLineContainer[nowTag].begin();
		for (; iter != m_AtkLineContainer[nowTag].end(); ) {
			if ((*iter)->isActive() == false) {
				iter = m_AtkLineContainer[nowTag].erase(iter);
			}
			else {
				++iter;
			}
		}
	}
}

void CCollisionMgr::Render(HDC _hDC)
{
	if (m_bRender) {
		RenderAtkBox(MONSTER, _hDC);
		RenderAtkBox(PLAYER, _hDC);

		RenderAtkLine(PLAYER, _hDC);
		RenderAtkLine(MONSTER, _hDC);

		RenderCollider(PLATFORM, _hDC);
		RenderCollider(PLAYER, _hDC);
		RenderCollider(MONSTER, _hDC);
		RenderCollider(PROJECTILE, _hDC);
		RenderCollider(ITEM, _hDC);
		RenderCollider(PARTICLE, _hDC);
	}
}

void CCollisionMgr::Release()
{
}


void CCollisionMgr::RenderCollider(OBJ_TAG tag, HDC _hDC)
{
	const auto& colVec = CObjMgr::Get_Instance()->Get_ObjectVector(tag);

	for (CObject* obj : colVec) {
		if (obj->Get_Coll_Active() == false) continue;

		CCollider* Col = obj->Get_Collider();

		if (Col) {
			Col->Render(_hDC);
		}
	}
}

void CCollisionMgr::RenderAtkBox(OBJ_TAG tag, HDC _hDC)
{
	if (m_AtkBoxContainer[tag].empty()) return;

	for (CAttackBox* box : m_AtkBoxContainer[tag]) {
		if (box) {
			box->Render(_hDC);
		}
	}
}
void CCollisionMgr::RenderAtkLine(OBJ_TAG tag, HDC _hDC)
{
	if (m_AtkLineContainer[tag].empty()) return;

	for (CAttackLine* line : m_AtkLineContainer[tag]) {
		if (line) {
			line->Render(_hDC);
		}
	}
}
void CCollisionMgr::Register_AtkBox(CAttackBox* pAtkBox)
{
	if (!pAtkBox) return; // null 방지

	OBJ_TAG tag = pAtkBox->Get_Target();

	// 중복 등록 방지
	auto& container = m_AtkBoxContainer[tag];
	auto iter = std::find(container.begin(), container.end(), pAtkBox);

	if (iter == container.end()) {
		container.push_back(pAtkBox);
	}
}

void CCollisionMgr::Register_AtkLine(CAttackLine* _Atkline)
{
	if (!_Atkline) return; // null 방지

	OBJ_TAG tag = _Atkline->Get_Target();

	// 중복 등록 방지
	auto& container = m_AtkLineContainer[tag];
	auto iter = std::find(container.begin(), container.end(), _Atkline);

	if (iter == container.end()) {
		container.push_back(_Atkline);
	}
}POINT CCollisionMgr::Check_SafetyPlatform(CRader* Rader, float safeDistanceX, float safeDistanceY)
{
	vector<CObject*>& platformVec = CObjMgr::Get_Instance()->Get_ObjectVector(PLATFORM);
	INFO_EX raderZone = Rader->Get_Info();

	RECT raderRect;
	raderZone.Update_INFO(raderRect);

	CObject* bestTile = nullptr;
	float bestY = -FLT_MAX;

	for (CObject* Tile : platformVec)
	{
		if (!Tile->Is_Activate() || !Tile->Get_Collider() || !Tile->Get_Collider()->isActive())
			continue;

		INFO_EX tileInfo = Tile->Get_Collider()->Get_Info();
		RECT tileRect;
		tileInfo.Update_INFO(tileRect);

		// 1. 레이더 범위 내인지
		RECT tmpRect;
		if (!IntersectRect(&tmpRect, &raderRect, &tileRect))
			continue;

		// 2. 안전 공간 생성
		RECT safeRect;
		safeRect.left = (LONG)(tileInfo.LT().x - safeDistanceX);
		safeRect.right = (LONG)(tileInfo.RB().x + safeDistanceX);
		safeRect.top = (LONG)(tileInfo.LT().y - safeDistanceY);
		safeRect.bottom = (LONG)(tileInfo.LT().y);

		bool bHasObstacle = false;
		for (CObject* other : platformVec)
		{
			if (other == Tile || !other->Is_Activate() || !other->Get_Collider() || !other->Get_Collider()->isActive())
				continue;

			INFO_EX otherInfo = other->Get_Collider()->Get_Info();
			RECT otherRect;
			otherInfo.Update_INFO(otherRect);

			if (IntersectRect(&tmpRect, &safeRect, &otherRect))
			{
				bHasObstacle = true;
				break;
			}
		}

		if (!bHasObstacle)
		{
			if (tileInfo.fY > bestY) {
				bestTile = Tile;
				bestY = tileInfo.fY;
			}
		}
	}

	if (bestTile)
	{
		INFO_EX info = bestTile->Get_Collider()->Get_Info();
		return { (LONG)info.fX, (LONG)info.fY };
	}
	return { 0, 0 };
}


bool CCollisionMgr::Trigger_With_Player(CObject* Trriger)
{
	INFO_EX player =CObjMgr::Get_Instance()->ReQuestObject<CPlayer>(PLAYER)->Get_Collider()->Get_Info();
	INFO_EX obj = Trriger->Get_Info();

	RECT PLRECT;
	player.Update_INFO(PLRECT);
	RECT OBRECT;
	obj.Update_INFO(OBRECT);
	RECT TMPRECT;

	return IntersectRect(&TMPRECT, &PLRECT, &OBRECT);
}


void CCollisionMgr::CheckObjectOnGrounded(OBJ_TAG rightTag)
{
	vector<CObject*>& ObjVec = CObjMgr::Get_Instance()->Get_ObjectVector(rightTag);
	vector<CObject*>& platformVec = CObjMgr::Get_Instance()->Get_ObjectVector(PLATFORM);

	float correctY = 0.f;
	bool CheckGorund = false;

	for (CObject* Obj : ObjVec) { //물체
		if (!Obj->Is_Activate() || !Obj->Get_Collider() || !Obj->Get_Collider()->isActive()) continue;

		INFO_EX objInfo = Obj->Get_Collider()->Get_Info();
		Obj->Set_Grounded(false);
		auto iter = find_if(platformVec.begin(), platformVec.end(), //오브젝트 매니저가 관리하는 충돌 대상 벡터 받아옴
			[&objInfo, &correctY, &CheckGorund,&rightTag,&Obj](CObject* Tile) -> bool {
				if (!Tile->Is_Activate() || !Tile->Get_Collider() || !Tile->Get_Collider()->isActive()) return false;

				INFO_EX info = Tile->Get_Collider()->Get_Info();

				if (info.LT().x <= objInfo.fX && info.RB().x >= objInfo.fX) { //람다로 지금 x좌표 일치하는거 순회하는거
					if (info.LT().y <= objInfo.RB().y) { // 위 아래 가 겹쳤다면  -> 좌상단이 우하단보다 위에 있다면
						if (info.RB().y > objInfo.RB().y) {//우하단이 우하단보다 
							correctY = objInfo.RB().y - info.LT().y; //충돌 대상은 바로 너야!
							CheckGorund = true;
							return true; // 리턴
						}
						else if (info.RB().y >= objInfo.LT().y) {
							correctY = -1 * (info.RB().y - objInfo.LT().y); //충돌 대상은 바로 너야!
							if (rightTag == MONSTER)
								correctY = 0;
							CheckGorund = false;
							return true; // 리턴
						}
					}
				}
				return false;
			});

		if (iter != platformVec.end()) {
			Obj->Set_Pos(Obj->Get_Info().fX, Obj->Get_Info().fY - correctY);
			Obj->Set_Grounded(CheckGorund);
		}
	}
}

void CCollisionMgr::CheckObjectOnWall(OBJ_TAG rightTag)
{
	vector<CObject*>& ObjVec = CObjMgr::Get_Instance()->Get_ObjectVector(rightTag);
	vector<CObject*>& platformVec = CObjMgr::Get_Instance()->Get_ObjectVector(PLATFORM);

	for (CObject* Obj : ObjVec) {
		if (!Obj->Is_Activate() || !Obj->Get_Collider() || !Obj->Get_Collider()->isActive()) continue;

		INFO_EX objInfo = Obj->Get_Collider()->Get_Info();
		float correctX = 0.f;
		int dir = 0;
		auto iter = find_if(platformVec.begin(), platformVec.end(),
			[&objInfo, &correctX, &dir](CObject* Tile) -> bool {
				if (!Tile->Is_Activate() || !Tile->Get_Collider() || !Tile->Get_Collider()->isActive()) return false;

				INFO_EX info = Tile->Get_Collider()->Get_Info();

				// 오브젝트 중심 Y가 타일 Y 범위 내에 있는지
				if (objInfo.fY >= info.LT().y && objInfo.fY <= info.RB().y) {

					// 오른쪽 벽에 부딪힘 (오브젝트가 왼쪽에서 타일에 닿음)
					if (objInfo.RB().x >= info.LT().x && objInfo.fX < info.fX) {
						correctX = info.LT().x - objInfo.RB().x;
						dir = -1;
						return true;
					}
					// 왼쪽 벽에 부딪힘 (오브젝트가 오른쪽에서 타일에 닿음)
					else if (objInfo.LT().x <= info.RB().x && objInfo.fX > info.fX) {
						correctX = info.RB().x - objInfo.LT().x;
						dir = 1;
						return true;
					}
				}
				return false;
			});

		if (iter != platformVec.end()) {
			Obj->Set_Pos(Obj->Get_Info().fX + correctX, Obj->Get_Info().fY);
			Obj->OnCollisionWall(dir);
		}
	}
}

void CCollisionMgr::CheckRectAtkCollision(OBJ_TAG tag)
{
	// 공격 박스 컨테이너가 존재하지 않으면 리턴
	if (m_AtkBoxContainer.find(tag) == m_AtkBoxContainer.end()) return;

	vector<CObject*>& ObjVec = CObjMgr::Get_Instance()->Get_ObjectVector(tag);
	vector<CAttackBox*>& AtkVec = m_AtkBoxContainer[tag];

	for (CObject* obj : ObjVec)
	{
		if (!obj || !obj->Is_Activate()) continue;
		if (!obj->Get_Coll_Active()) continue;
		if (!obj->Get_Collider()) continue;
		// 객체 정보 갱신
		RECT objRect;
		//obj->Get_Info().Update_INFO(objRect);
		obj->Get_Collider()->Get_Info().Update_INFO(objRect);
		for (CAttackBox* box : AtkVec)
		{
			if (!box || !box->isActive()) continue;

			RECT boxRect;
			box->Get_Info().Update_INFO(boxRect);

			RECT tmpRect;
			if (IntersectRect(&tmpRect, &objRect, &boxRect))
			{
				if (box->Can_Hit(obj))
				{
					box->OnCollisionEnter(obj, tmpRect, ATTACK);
					obj->OnCollisionEnter(box->Get_Owner(), tmpRect, HITTED);
				}
			}
		}
	}
}

void CCollisionMgr::CheckLineAtkCollision(OBJ_TAG tag)
{
	// 공격 박스 컨테이너가 존재하지 않으면 리턴
	if (m_AtkLineContainer.find(tag) == m_AtkLineContainer.end()) return;

	vector<CObject*>& ObjVec = CObjMgr::Get_Instance()->Get_ObjectVector(tag);
	vector<CAttackLine*>& AtkVec = m_AtkLineContainer[tag];

	for (CObject* obj : ObjVec)
	{
		if (!obj || !obj->Is_Activate()) continue;
		if (!obj->Get_Coll_Active()) continue;
		if (!obj->Get_Collider()) continue;

		// 객체 정보 갱신
		RECT objRect;
		//obj->Get_Info().Update_INFO(objRect);
		obj->Get_Collider()->Get_Info().Update_INFO(objRect);
		for (CAttackLine* line : AtkVec)
		{
			if (!line || !line->isActive()) continue;

			LINE_INFO line_info;
			line_info = line->Get_LineInfo();
			RECT tmpRect{};
			if (line->InterSectLine(objRect))
			{
				if (line->Can_Hit(obj))
				{
					line->OnCollisionEnter(obj, tmpRect, ATTACK);
					obj->OnCollisionEnter(line->Get_Owner(), tmpRect, HITTED);
				}
			}
		}
	}
}
