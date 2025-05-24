#pragma once
#include "CObject.h"
#include "CAnim.h"
#include "MonsterStates.h"

class CAnim;
class CMonsterState;
class CRader;

class CMonster :
    public CObject
{
public:
    CMonster();
    ~CMonster();
public:
   virtual void Find_Target(CObject* target) PURE;
   virtual void Not_Find_Target() PURE;
   CAnim* Get_Anim() { return m_pAnim; };
   void SetAnim();
   void ChangeState(MONSTER_STATE state);
   MONSTER_STATE Get_State() { return m_nowState; }
   void Anim_Direction();
   void ChangeAnimPath(wstring path);
   void Set_Target(CObject* target) { m_pTarget = target; }
   CObject* Get_Target() { return m_pTarget; }
   void ReleaseTarget() { m_bTargeting = false;}
   void Get_Damage(float Dmg);
   CRader* Get_Rader() { return m_pRader; }
   bool Can_Attack() { return m_bAttack; }
   virtual void Attacked();
   void Set_Render(bool Render) { m_bRender = Render; }
   void Set_DirStick(bool stick) { m_bStickDir = stick; };
   wstring Get_Name() {return m_MonsterName;}
protected:
    bool m_bRender;
    bool m_bStickDir;
    bool m_bTargeting;
    bool m_bAttack;
    float m_MaxHp;
    float m_NowHp;
    CAnim* m_pAnim;
    CRader* m_pRader;
    CObject* m_pTarget;
    wstring m_MonsterName;
    unordered_map<MONSTER_STATE, CMonsterState*> m_StateContainer;
    MONSTER_STATE m_nowState;
};

