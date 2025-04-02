#include "pch.h"
#include "CObject.h"
#include "CItem.h"

CObject::CObject()
	:m_Sname{}, m_Stat(), m_AtkType(0),m_State(0), nowHp(0)
{
	//L"화상" {Turn,Damage}
	m_stateContainer.insert({ STATE_KEY::BURN ,{0,0}});
	m_stateContainer.insert({ STATE_KEY::POISON ,{0,0} });
}

CObject::~CObject()
{
}

void CObject::Initialize()
{
}

void CObject::Render()
{
}

void CObject::Update()
{
}

void CObject::Release()
{
}

void CObject::GetDamage(CObject* obj)
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 100);

	if (dis(gen) < m_Stat.iAvd) {
		wcout << m_Sname <<L"은 " << obj->GetName() << L" 의 공격을 완전히 회피했다!" << endl;
		return;
	}
	
	int AtkTmpType = obj->GetAtkType();	//공격 상대의 어택 타입
	int AtkTmp = 0;													//데미지 계산 변수
	
	if (AtkTmpType & NormalAtk) {
		wcout << obj->GetName() << L" -> "<<m_Sname<<L"  일반 공격!" << endl;
		AtkTmp=obj->GetInfo().iNormAtk;
		AtkTmp -= m_Stat.iNormDef;
	}
	if (AtkTmpType & MagicalAtk) {
		wcout << obj->GetName() << L" -> " << m_Sname << L"  마법 공격!" << endl;
		AtkTmp=obj->GetInfo().iMgAtk;
		AtkTmp -= m_Stat.iMgDef;
	}

	if (dis(gen) < m_Stat.iCrit) {
		AtkTmp += static_cast<int>((m_Stat.iCritDmg / 100.0) * AtkTmp); //크리티컬 계산
	}

	nowHp -= AtkTmp;

	if (AtkTmpType & BurnAtk) {
		GetDebuff(BurnAtk);		//속성 부여
	}
	if (AtkTmpType & PoisonAtk) {
		GetDebuff(PoisonAtk); //속성 부여
	}

	CheckState();
}

void CObject::GetDebuff(int _AtkType, int turn, int damage)
{
	
	if (_AtkType & BurnAtk) {
		if (m_State |= BurnAtk) {//화상 중첨 => 데미지 증가
			m_stateContainer[STATE_KEY::BURN][0] += turn;//턴
			m_stateContainer[STATE_KEY::BURN][1] += damage; //데미지
		}
		else {
			m_stateContainer[STATE_KEY::BURN][1] += damage;
		}
	}

	if (_AtkType & PoisonAtk) { //중독 중첨 => 턴 증가
		if (m_State |= PoisonAtk) {
			m_stateContainer[STATE_KEY::POISON][0] += turn;//턴
			m_stateContainer[STATE_KEY::POISON][1] += damage; //데미지
		}
		else {
			m_stateContainer[STATE_KEY::POISON][0] += turn;//턴
			m_stateContainer[STATE_KEY::POISON][1] = damage; //데미지
		}
	}

}

int CObject::GetAtkType()
{
	return m_AtkType;
}

wstring CObject::RenderState()
{
	if (m_State == 0) {
		return L"";
	}

	wstring tmp;
	tmp += L"(";

	if (m_State & BurnAtk) {
		tmp += (L"화상 :" + to_wstring(m_stateContainer[STATE_KEY::BURN][0]));
	}
	if (m_State & PoisonAtk) {
		tmp += (L"중독 :" + to_wstring(m_stateContainer[STATE_KEY::POISON][0]));
	}
	tmp += L")";
	
	return tmp;
}

void CObject::CheckState()
{
	unordered_map<STATE_KEY, vector<int>>::iterator iter = m_stateContainer.begin();
	for (;iter != m_stateContainer.end();++iter) {
		vector<int>&tmp = iter->second;
		if (tmp[0] > 0) {//턴
			--tmp[0];
			nowHp -= tmp[1];//데미지
		}
		if (tmp[0] <= 0) {
			tmp[0] = 0;
			m_State &= ~(static_cast<int>(iter->first));
		}
	}
}

void CObject::Heal(int point)
{
	if (point + nowHp > m_Stat.iMaxHp) {
		nowHp = m_Stat.iMaxHp;
	}
	else {
		nowHp += point;
	}
}

void CObject::StateRestore()
{
	unordered_map<STATE_KEY, vector<int>>::iterator iter = m_stateContainer.begin();

	for (;iter != m_stateContainer.end();++iter) {
		vector<int>& tmp = iter->second;
		tmp[0] = 0;
		tmp[1] = 0;
	}
}

void CObject::StateRestore(STATE_KEY key)
{
	m_stateContainer[key][0] = 0;
	m_stateContainer[key][1] = 0;
}

const wstring& CObject::GetName()
{
	return m_Sname;
}
