#include "pch.h"
#include "CObject.h"
#include "CItem.h"

CObject::CObject()
	:m_Sname{}, m_Stat(), m_AtkType(0),m_State(0), nowHp(0)
{
	m_stateContainer.insert({ L"화상", {0,0} });
	m_stateContainer.insert({ L"중독", {0,0} });
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
	CheckState();
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
	
	int AtkTmpType = obj->GetAtkType();
	int AtkTmp = 0;

	if (AtkTmpType & NormalAtk) {
		wcout << obj->GetName() << L"-> "<<m_Sname<<L"  일반 공격!" << endl;
		AtkTmp=obj->GetInfo().iNormAtk;
	}
	if (AtkTmpType & MagicalAtk) {
		wcout << obj->GetName() << L"-> " << m_Sname << L"  마법 공격!" << endl;
		AtkTmp=obj->GetInfo().iMgAtk;
	}
	if (dis(gen) < m_Stat.iCrit) {
		AtkTmp += static_cast<int>((m_Stat.iCritDmg / 100.0) * AtkTmp);
	}

	nowHp -= AtkTmp;

	if (AtkTmpType & BurnAtk) {
		GetDebuff(BurnAtk);
	}

	if (AtkTmpType & PoisonAtk) {
		GetDebuff(PoisonAtk);
	}
}

void CObject::GetDebuff(int _AtkType)
{
	if (_AtkType & BurnAtk) {
		m_State |= BurnAtk;
	}
	if (_AtkType & PoisonAtk) {
		m_State |= PoisonAtk;
	}
}

int CObject::GetAtkType()
{
	return m_AtkType;
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

const wstring& CObject::GetName()
{
	return m_Sname;
}

void CObject::CheckState()
{
	if (m_State & BurnAtk) {
		if (0 < m_stateContainer[L"화상"][0]) 
		{ //이미 화상이 걸려 있다면.
			--m_stateContainer[L"화상"][0]; //턴 차감 
			m_stateContainer[L"화상"][1] += 2; // 데미지 증가
		}
		else 
		{//이미 화상이 걸려 있지 않다면
			wcout << m_Sname << L"가  화상 상태!" << endl;
			m_stateContainer[L"화상"][0] =3;  //3번
			m_stateContainer[L"화상"][1] = 2; //2데미지
		}
		
		nowHp -= m_stateContainer[L"화상"][1]; 
		wcout << m_Sname << L"가 " << m_stateContainer[L"화상"][1] << L"만큼의 화상 피해!" << endl;

		if (m_stateContainer[L"화상"][0] == 0) {
			m_State &= ~BurnAtk; //화상 턴이 끝나면
			wcout << m_Sname << L"의 상태이상 해제!" << endl;
		}
	}

	if (m_State & PoisonAtk) {

		if (0 < m_stateContainer[L"중독"][0]) {
			--m_stateContainer[L"중독"][0];
		}
		else {
			m_stateContainer[L"중독"][0] = 2;
			m_stateContainer[L"중독"][1] = 5;
			wcout << m_Sname << L"가  중독 상태!" << endl;
		}

		nowHp -= m_stateContainer[L"중독"][1];
		wcout << m_Sname << L"가 " << m_stateContainer[L"중독"][1] << L"만큼의 중독 피해!" << endl;

		if (m_stateContainer[L"중독"][0] == 0) {
			m_State &= ~PoisonAtk;
			wcout << m_Sname << L"의 상태이상 해제!" << endl;
		}
	}
}

wstring CObject::RenderState()
{
	if (m_State == 0) {
		return L"";
	}

	wstring tmp;
	tmp += L"(";

	if (m_State & BurnAtk) {
		tmp += (L"화상 :" + to_wstring(m_stateContainer[L"화상"][0]));
	}
	if (m_State & PoisonAtk) {
		tmp += (L"중독 :" + to_wstring(m_stateContainer[L"중독"][0]));
	}
	tmp += L")";
	
	return tmp;
}
