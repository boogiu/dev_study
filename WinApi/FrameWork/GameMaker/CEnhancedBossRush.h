#pragma once
#include "CBossState.h"
class CBaseEffect;
class CDimensionPierce;
class CAttackBox;


struct tagLaserEvent {
	INFO info;
	float angleRad;
	POINT frameCoord;
	bool bFired;
	CDimensionPierce* pLaser;
	float elapsed;  
	tagLaserEvent(
		const INFO& _info,
		float _angleRad,
		const POINT& _frameCoord,
		bool _bFired = false,
		CDimensionPierce* _pLaser = nullptr,
		float _elapsed = 0.f
	)
		: info(_info), angleRad(_angleRad), frameCoord(_frameCoord), bFired(_bFired), pLaser(_pLaser), elapsed(_elapsed)
	{
	}
};


class CEnhancedBossRush :
    public CBossState
{
public:
    CEnhancedBossRush();
    ~CEnhancedBossRush();
public:
    void Enter(CBoss* boss) override;
    void Update(CBoss* boss) override;
    bool Exit(CBoss* boss) override;


private:
	void MoveToTarget(CBoss* boss);
    CDimensionPierce* Create_Laser(CBoss* boss);
	void FirstAtk(CBoss* boss);
	void SecontAtk(CBoss* boss);
	void Smoke(CBoss* boss);
	void Active_All(CBoss* boss);
	void Fire_All(CBoss* boss);

private:
	enum state {ENTER, CHASETARGET, FIRSTATK,SECONDATK, LASERACTIVE, LASERFIRE,ENDSTATE};
	state m_eNowState;
	int AttackCount;
	bool AttackSoundA;
	bool AttackSoundB;
	bool m_bActiveAll;
	bool m_telPor_in;
	float m_elapseTime;//난사 속도 조절
	float m_patternDelay;//패턴 속도 조절
	vector<bool> m_patternFlag;
    vector<tagLaserEvent> m_Laservec;
	CAttackBox* m_AtkBox;
	int prevDir;
};
