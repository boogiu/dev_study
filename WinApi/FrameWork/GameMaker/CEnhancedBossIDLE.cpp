#include "pch.h"
#include "CEnhancedBossIDLE.h"

CEnhancedBossIDLE::CEnhancedBossIDLE()
{
	aniFrame.m_spSize = { 300,200 };
	aniFrame.startPoint = { 1,0 };
	aniFrame.endPoint = { 4,0 };
	aniFrame.isLoop = true;
	aniFrame.LoopPoint = { 0,0 };
	aniFrame.frameSpeed = 0.2f;
}

CEnhancedBossIDLE::~CEnhancedBossIDLE()
{
	
}

void CEnhancedBossIDLE::Enter(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(50.f, 50.f);
	boss->SetAnim(L"IDLE");
	boss->Set_IDLE_END(false);
}

void CEnhancedBossIDLE::Update(CBoss* boss)
{
	boss->Set_IDLE_END(true);
}

bool CEnhancedBossIDLE::Exit(CBoss* boss)
{
	boss->Get_Info().Add_OffsetX(-50.f, -50.f);
    return false;
}
