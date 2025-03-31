#include "pch.h"
#include "CPlayer.h"
#include "CInventory.h"
#include "Items.h"

CPlayer::CPlayer()
	:m_Class(CLASS::NONE), m_inven(nullptr), m_MaxExp(10), m_BonusStat(0)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	if (!m_inven) {
		m_inven = new CInventory;
		m_inven->Initialize();
	}
}

void CPlayer::Update()
{

	CheckState();

}

void CPlayer::Render()
{
	wcout << m_Sname << RenderState() << endl;
	wcout << L"-----------" << endl;
	wcout << L"체력 : " << nowHp << L" / " << m_Stat.iMaxHp << endl;
}

void CPlayer::Release()
{
	if (m_inven) {
		m_inven->Release();
		SAFE_DELETE(m_inven);
	}
}

void CPlayer::SetName()
{
	while (true)
	{
		wcout << L"사용할 이름을 입력해주세요." << endl;
		m_Sname = CInput::SafeChar();
		wcout << L"당신의 이름은 " << m_Sname << L"입니다." << endl;
		wcout << L"1. 확인 \t 2. 다시 설정" << endl;
		if (CInput::SafeInt(1, 2) == 1) {
			break;
		}
	}

}

void CPlayer::SetClass(tagStatus stat, CLASS _Class)
{
	m_Stat = stat;
	m_Class = _Class;

	switch (m_Class)
	{
	case CLASS::NONE:
		break;

	case CLASS::WARRIOR:
		m_AtkType |= NormalAtk;
		break;

	case CLASS::MAGE:
		m_AtkType |= MagicalAtk;
		m_AtkType |= BurnAtk;
		break;

	case CLASS::ROGUE:
		m_AtkType |= NormalAtk;
		m_AtkType |= PoisonAtk;
		break;

	case CLASS::ARCHOR:
		m_AtkType |= NormalAtk;
		break;

	default:
		break;
	}
	nowHp = m_Stat.iMaxHp;
}

void CPlayer::ShowStatus()
{
	while (true)
	{
		system("cls");
		wcout << GetName() << L" [" << ReturnClass(m_Class) << L"]" << endl;
		wcout << L"---------------------" << endl;
		wcout << L"1. 체력" << nowHp << L"/" << m_Stat.iMaxHp << endl;
		wcout << L"2. 일반 공격력" << m_Stat.iNormAtk << endl;
		wcout << L"3. 마법 공격력" << m_Stat.iMgAtk << endl;
		wcout << L"4. 크리티컬 확률" << m_Stat.iCrit << L"%" << endl;
		wcout << L"5. 크리티컬 데미지" << m_Stat.iCritDmg + 100 << L"%" << endl;
		wcout << L"6. 방어력" << m_Stat.iNormDef << endl;
		wcout << L"7. 마법 방어력" << m_Stat.iMgDef << endl;
		wcout << L"8. 회피율" << m_Stat.iAvd << L"%" << endl;
		wcout << L"---------------------" << endl;
		wcout << L"경험치" << m_Stat.iExp << L"/" << m_MaxExp << endl;
		wcout << L"소지금" << m_Stat.iGold << L"G" << endl;
		wcout << L"==========================================" << endl;
		wcout << L"보너스 스탯 : " << m_BonusStat << endl;
		wcout << endl;
		wcout << L"0.나가기 \t" << endl;
		wcout << L"> 강화할 스탯의 번호를 입력" << endl;
		int iSelcet = CInput::SafeInt(0, 7);
		if (iSelcet == 0) return;
		if (m_BonusStat > 0) {
			switch (iSelcet) {
			case 1:
				m_Stat.iMaxHp += 10;
				wcout << L"최대 체력 " << 10 << L"상승" << endl;
				break;
			case 2:
				m_Stat.iNormAtk += 5;
				wcout << L"일반 공격 " << 5 << L"상승" << endl;
				break;
			case 3:
				m_Stat.iMgAtk += 5;
				wcout << L"마법 공격 " << 5 << L"상승" << endl;
				break;
			case 4:
				m_Stat.iCrit += 5;
				wcout << L"크리티컬 확률 " << 5 << L"% 상승" << endl;
				break;
			case 5:
				m_Stat.iCritDmg += 5;
				wcout << L"크리티컬 데미지" << 5 << L"% 상승" << endl;
				break;
			case 6:
				m_Stat.iNormDef += 5;
				wcout << L"방어력 " << 5 << L"상승" << endl;
				break;
			case 7:
				m_Stat.iMgDef += 5;
				wcout << L"마법 방어력 " << 5 << L"상승" << endl;
				break;
			case 8:
				m_Stat.iAvd += 2;
				wcout << L"회피율 " << 2 << L"% 상승" << endl;
				break;
			}
			system("pause");
		}
		else {
			wcout << L"보너스 스탯이 없습니다." << endl;
			system("pause");
		}
	}
}

void CPlayer::ShowInventory()
{
	system("cls");
	m_inven->ShowInven();
	wcout << L"0. 나가기" << endl;
	int itemSelcet = CInput::SafeInt(0, 0);
	if (itemSelcet == 0) {
		system("cls");
		return;
	}
}

void CPlayer::ShowInventory(CObject* compete)
{
	while (true)
	{
		m_inven->ShowInven();
		wcout << L"0. 나가기" << endl;
		int itemSelcet = CInput::SafeInt(0, m_inven->GetSlotCount());
		system("cls");

		if (itemSelcet == 0) {
			return;
		}
		--itemSelcet;

		if (!m_inven->CheckItemPtr(itemSelcet)) {
			wcout << L"아이템 존재하지 않음" << endl;
			system("pause");
			continue;
		}

		if (dynamic_cast<CThrow*>(m_inven->CheckItemPtr(itemSelcet))) {
			m_inven->UseSlot(itemSelcet, compete);
		}
		else {
			m_inven->UseSlot(itemSelcet, this);
		}
		wcout << m_Sname << L"의"<< m_inven->CheckItemPtr(itemSelcet)->GetName() <<L" 사용!" << endl;
		break;
	}
}

void CPlayer::ShowSkill()
{
	system("cls");
	wcout << L"아직 구현 못함..." << endl;
	wcout << L"1. 확인 \t" << endl;
	int iSelcet = CInput::SafeInt(1, 1);
}

int CPlayer::GetAtkType()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 100);

	if (dis(gen) < 6) {
		return m_AtkType;
	}
	return (m_AtkType & (~PoisonAtk)) & (m_AtkType & (~BurnAtk));
}

void CPlayer::Purchase(CItem* item)
{
	if (m_Stat.iGold < item->GetCost()) {
		wcout << L"소지금이 부족합니다." << endl;
		system("pause");
		return;
	}
	else {
		if (m_inven->PushItem(item)) {
			m_Stat.iGold -= item->GetCost();
		}
	}
}

void CPlayer::GetProps(CObject* object)
{
	m_Stat.iGold += object->GetInfo().iGold;
	GetExp(object->GetInfo().iExp);
}

void CPlayer::UseSlot(int index, CObject* target)
{
	if (!m_inven->UseSlot(index, target)) {
		wcout << "슬롯을 사용할 수 없습니다." << endl;
	}
}

void CPlayer::GetExp(int exp)
{
	m_Stat.iExp += exp;	//얻은 경험치 + 현재 경험치가

	while (m_Stat.iExp >= m_MaxExp) {
		m_Stat.iExp -= m_MaxExp;
		LevelUp(); // 레벨업 실행
	}
}

void CPlayer::LevelUp()
{
	++m_BonusStat;
	m_MaxExp += 5;
}

wstring CPlayer::ReturnClass(CLASS _class)
{
	switch (_class)
	{
	case CLASS::NONE:
		return L"백수";
	case CLASS::WARRIOR:
		return L"전사";
	case CLASS::MAGE:
		return L"마법사";
	case CLASS::ROGUE:
		return L"도적";
	case CLASS::ARCHOR:
		return L"궁수";
	default:
		break;
	}
}

