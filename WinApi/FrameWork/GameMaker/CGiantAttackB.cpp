#include "pch.h"
#include "CGiantAttackB.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CEffectMgr.h"
#include "CAttackBox.h"
#include "CResourceMgr.h"

CGiantAttackB::CGiantAttackB()
	:m_WaveCnt(0), m_WaveDelay(0.f), m_ElapsedTime(0.f), m_bWaveStart(false)
	, m_bCheckDir(false), m_iCheckedDir(0), effectFrame{}, m_tAtkInfo{}, m_atkArr{ nullptr }
{
	aniFrame.startPoint = { 0,0 };
	aniFrame.endPoint = { 3,1 };
	aniFrame.isLoop = false;
	aniFrame.frameSpeed = 0.2f;
	aniFrame.m_spSize = { 200,150 };

	effectFrame.startPoint = { 0,0 };
	effectFrame.endPoint = { 4,4 };
	effectFrame.isLoop = false;
	effectFrame.frameSpeed = 0.02f;
	effectFrame.m_spSize = { 150,150 };
}

CGiantAttackB::~CGiantAttackB()
{
	for (int i = 0; i < 5; ++i) {
		if (m_atkArr[i]) {
			Safe_Delete<CAttackBox*>(m_atkArr[i]);
		}
	}
}

void CGiantAttackB::Enter(CMonster* monster)
{
	for (int i = 0; i < 5; ++i) {
		if (!m_atkArr[i]) {
			m_atkArr[i] = new CAttackBox(monster);
			m_atkArr[i]->Set_Target(PLAYER);
		}
	}

	monster->Set_Velocity(0.f, 0.f);
	monster->SetAnim();
	m_bWaveStart = false;
	
	m_bCheckDir = false;
	m_iCheckedDir = 0;
	
	m_WaveCnt = 0;
	m_ElapsedTime = 0.5f;
	m_WaveDelay = 0.5f;
	
	m_tAtkInfo = { 0, 0, 150,150 };
}

void CGiantAttackB::Update(CMonster* monster)
{
	if (!m_bCheckDir) { //시작하면 방향 고정
		m_bCheckDir = true;
		m_iCheckedDir = monster->Get_Dir();
		monster->Set_DirStick(true);
	}

	if (monster->Get_Anim()->Calc_NowFrame({ 3,0 })) {
		m_bWaveStart = true; //내려 찍는 모션 나오면 웨이브 시작
	}

	if (m_bWaveStart && m_WaveCnt < 5) {
		m_ElapsedTime += CTimeMgr::Get_Instance()->Get_Delta();
		if (m_ElapsedTime > m_WaveDelay) {//5가 되면 그만
			m_tAtkInfo.fY = monster->Get_Collider()->Get_Info().RB().y+20;
			m_tAtkInfo.fX = monster->Get_Collider()->Get_Info().fX + (m_iCheckedDir * 5 * (20 + 20 * m_WaveCnt));
			m_tAtkInfo.fCX = m_tAtkInfo.fCX +(30 * (m_WaveCnt));
			m_tAtkInfo.fCY = m_tAtkInfo.fCY +(30 * (m_WaveCnt));
			//웨이브 생성
			Create_Wave();
			//웨이브 카운트 증가
			m_WaveCnt += 1;
			m_ElapsedTime = 0;
		}
	}

	if (monster->Get_Anim()->isAnimEnd() && m_WaveCnt >=5) {
		monster->Set_DirStick(false);
		monster->ChangeState(MONSTER_IDLE);
	}
}

bool CGiantAttackB::Exit(CMonster* monster)
{
	return false;
}

void CGiantAttackB::Create_Wave()
{
	CBaseEffect* Wave = dynamic_cast<CBaseEffect*>(CEffectMgr::Get_Instance()->ReQuestEffect<CBaseEffect>(NONE_DIRECTION));
	Wave->Initialize();
	Wave->Set_Dir(m_iCheckedDir);
	Wave->PlayAnim(L"Giant_Impact", effectFrame);
	CResourceMgr::Get_Instance()->PlaySoundW(L"GoldenmaneMAA_PowerWave", 0.15f * (m_WaveCnt + 1));

	Wave->Get_Info().Translate_Info(m_tAtkInfo);
	Wave->Get_Info().SetFromBottom(m_tAtkInfo.fY);
	Wave->Set_AutoDeaActive(true);

	INFO atkInfo = { m_tAtkInfo.fX,m_tAtkInfo.fY, m_tAtkInfo.fCX * 0.6,m_tAtkInfo.fCY * 0.6 };
	m_atkArr[m_WaveCnt]->Set_Active();
	m_atkArr[m_WaveCnt]->Get_Info().Translate_Info(atkInfo);
	m_atkArr[m_WaveCnt]->Get_Info().SetFromBottom(m_tAtkInfo.fY);
	Wave->Sync_AtkBox(m_atkArr[m_WaveCnt]);
}
