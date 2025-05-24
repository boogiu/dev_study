#include "pch.h"
#include "CResourceMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
CResourceMgr* CResourceMgr::m_pInstance = nullptr;

CResourceMgr::CResourceMgr()
	: m_BmpMgr(nullptr), m_SoundMgr(nullptr)
{
}

CResourceMgr::~CResourceMgr()
{
	Release();
}

void CResourceMgr::Initialize()
{
	if (!m_BmpMgr) {
		m_BmpMgr = new CBmpMgr;
		m_BmpMgr->Initialize();
	}
	if (!m_SoundMgr) {
		m_SoundMgr = new CSoundMgr;
		m_SoundMgr->Initialize();
	}
	Load_Resource();
}

void CResourceMgr::Update()
{
	if (m_SoundMgr) {
		m_SoundMgr->Update();
	}
}

void CResourceMgr::Late_Update()
{
}

void CResourceMgr::Render(HDC _hDC)
{
}

void CResourceMgr::Release()
{
	if (m_BmpMgr) {
		delete m_BmpMgr;
		m_BmpMgr = nullptr;
	}
	if (m_SoundMgr) {
		delete m_SoundMgr;
		m_SoundMgr = nullptr;
	}
}

HDC CResourceMgr::Find_Image(const TCHAR* pImageKey)
{
	if (m_BmpMgr) {
		return m_BmpMgr->Find_Image(pImageKey);
	}
	return nullptr;
}

void CResourceMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey)
{
	if (m_BmpMgr) {
		m_BmpMgr->Insert_Bmp(pFilePath, pImageKey);
	}
}


void CResourceMgr::PlaySound(const wstring& pSoundKey, float fVolume, int* ID, bool Loop)
{
	if (m_SoundMgr) {
		m_SoundMgr->PlaySound(pSoundKey, fVolume, ID, Loop);
	}
}

void CResourceMgr::PlaySound(const wstring& pSoundKey, float fVolume, bool Loop)
{
	if (m_SoundMgr) {
		m_SoundMgr->PlaySound(pSoundKey, fVolume, Loop);
	}
}

void CResourceMgr::PlayBGM(const wstring& pSoundKey, float fVolume)
{
	if (m_SoundMgr) {
		m_SoundMgr->PlayBGM(pSoundKey, fVolume);
	}
}


void CResourceMgr::StopSound(CHANNELID eID)
{
	if (m_SoundMgr) {
		m_SoundMgr->StopSound(eID);
	}
}

void CResourceMgr::StopSound(int eID)
{
	if (m_SoundMgr) {
		m_SoundMgr->StopSound(eID);
	}
}

void CResourceMgr::StopAll()
{
	if (m_SoundMgr) {
		m_SoundMgr->StopAll();
	}
}

void CResourceMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (m_SoundMgr) {
		m_SoundMgr->SetChannelVolume(eID, fVolume);
	}
}

void CResourceMgr::Load_Resource()
{
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"backBuffer");
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------배경---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Tile");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/tile_32_4.bmp", L"Platform");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/BackOrnament.bmp", L"Ornament");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Stage_Door.bmp", L"Stage_Door");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile/Skul_Box.bmp", L"Skul_Box");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss_BackGround.bmp", L"Boss_BackGround");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/BackWall.bmp", L"BackWall");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/BackCutton.bmp", L"BackCutton");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/BackBridge.bmp", L"BackBridge");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Defualt_Back.bmp", L"Ground");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Boss_Back.bmp", L"Boss_Back");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Title_Logo.bmp", L"Title_Logo");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/Title_Logo2.bmp", L"Title_Logo2");

	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------스컬---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Base_Skul_RIGHT.bmp", L"Base_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Base_Skul_LEFT.bmp", L"Base_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/NoHead_Skul_RIGHT.bmp", L"NoHead_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/NoHead_Skul_LEFT.bmp", L"NoHead_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_RIGHT.bmp", L"Ent_Skul_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_LEFT.bmp", L"Ent_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_Skill_LEFT.bmp", L"Ent_Skul_Skill_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ent_Skul_Skill_RIGHT.bmp", L"Ent_Skul_Skill_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Wolf_Skul_LEFT.bmp", L"Wolf_Skul_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Wolf_Skul_RIGHT.bmp", L"Wolf_Skul_R");
	
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------UI---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Skul_ICON.bmp", L"Skul_ICON");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ent_ICON.bmp", L"Ent_ICON");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Wolf_ICON.bmp", L"Wolf_ICON");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Base_Skill_icon.bmp", L"Base_Skill_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Base_Skill2_icon.bmp", L"Base_Skill2_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Player_Frame.bmp", L"Player_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Player_HealthBar.bmp", L"Player_HealthBar");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/S_Btn.bmp", L"S_Btn");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/A_Btn.bmp", L"A_Btn");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Skill2_Frame.bmp", L"Skill2_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Subskull_Frame.bmp", L"Subskull_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/BossHealthBar_FirstPhase.bmp", L"BossHealthBar_FirstPhase");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Boss_HealthBar_Front.bmp", L"Boss_HealthBar_Front");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ch2BossFirstPhase_Top_Back.bmp", L"Ch2BossFirstPhase_Top_Back");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ch2BossFirstPhase_Bottom_Back.bmp", L"Ch2BossFirstPhase_Bottom_Back");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ch2BossSecondPhase_Front.bmp", L"Ch2BossSecondPhase_Front");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ch2BossSecondPhase_Top_Back.bmp", L"Ch2BossSecondPhase_Top_Back");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ch2BossSecondPhase_Bottom_Back.bmp", L"Ch2BossSecondPhase_Bottom_Back");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/BossHealthBar_SecondPhase.bmp", L"BossHealthBar_SecondPhase");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ent_Skill2_icon.bmp", L"Ent_Skill2_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Ent_Skill_icon.bmp", L"Ent_Skill_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Wolf_Skill_icon.bmp", L"Wolf_Skill_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Wolf_Skill2_icon.bmp", L"Wolf_Skill2_icon");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Popup_Frame.bmp", L"Popup_Frame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/NPC_TwinOgre.bmp", L"NPC_TwinOgre");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Dialogue_Name.bmp", L"Dialogue_Name");
	
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/A_Key.bmp", L"A_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/S_Key.bmp", L"S_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/D_Key.bmp", L"D_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/Z_Key.bmp", L"Z_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/X_Key.bmp", L"X_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/C_Key.bmp", L"C_Key");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/ui/SPACE_Key.bmp", L"SPACE_Key");


	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------몬스터---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_WAIT_L.bmp", L"BossA_WAIT_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_WAIT_L.bmp", L"BossB_WAIT_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_WAIT_L.bmp", L"BossA_WAIT_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_WAIT_L.bmp", L"BossB_WAIT_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_INTRO_L.bmp", L"BossA_INTRO_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_INTRO_L.bmp", L"BossB_INTRO_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_INTRO_R.bmp", L"BossA_INTRO_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_INTRO_R.bmp", L"BossB_INTRO_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_IDLE_L.bmp", L"BossA_IDLE_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_IDLE_L.bmp", L"BossB_IDLE_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_IDLE_R.bmp", L"BossA_IDLE_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_IDLE_R.bmp", L"BossB_IDLE_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_GROUND_L.bmp", L"BossA_GROUND_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_GROUND_L.bmp", L"BossB_GROUND_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_GROUND_R.bmp", L"BossA_GROUND_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_GROUND_R.bmp", L"BossB_GROUND_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_METEOR_L.bmp", L"BossA_METEOR_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_METEOR_L.bmp", L"BossB_METEOR_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_METEOR_R.bmp", L"BossA_METEOR_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_METEOR_R.bmp", L"BossB_METEOR_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_HOMING_L.bmp", L"BossA_HOMING_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_HOMING_L.bmp", L"BossB_HOMING_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_HOMING_R.bmp", L"BossA_HOMING_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_HOMING_R.bmp", L"BossB_HOMING_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_SLASH_L.bmp", L"BossA_SLASH_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_SLASH_L.bmp", L"BossB_SLASH_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_SLASH_R.bmp", L"BossA_SLASH_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_SLASH_R.bmp", L"BossB_SLASH_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_DEAD_L.bmp", L"BossA_DEAD_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_DEAD_L.bmp", L"BossB_DEAD_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossA_DEAD_R.bmp", L"BossA_DEAD_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossB_DEAD_R.bmp", L"BossB_DEAD_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_INTRO_L.bmp", L"BossC_INTRO_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_INTRO_R.bmp", L"BossC_INTRO_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_GROUND_L.bmp", L"BossC_GROUND_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_GROUND_R.bmp", L"BossC_GROUND_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_DEAD_L.bmp", L"BossC_DEAD_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_DEAD_R.bmp", L"BossC_DEAD_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_HOMING_L.bmp", L"BossC_HOMING_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_HOMING_R.bmp", L"BossC_HOMING_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_IDLE_L.bmp", L"BossC_IDLE_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_IDLE_R.bmp", L"BossC_IDLE_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_METEOR_L.bmp", L"BossC_METEOR_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_METEOR_R.bmp", L"BossC_METEOR_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_RUSH_L.bmp", L"BossC_RUSH_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_RUSH_R.bmp", L"BossC_RUSH_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_SLASH_L.bmp", L"BossC_SLASH_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BossC_SLASH_R.bmp", L"BossC_SLASH_R");


	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Enemy_Appearance.bmp", L"Enemy_Appearance");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Enemy_Dead.bmp", L"Enemy_Dead");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Soldier_L.bmp", L"Soldier_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Soldier_R.bmp", L"Soldier_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Archor_L.bmp", L"Archor_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Archor_R.bmp", L"Archor_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Giant_Soldier_R.bmp", L"Giant_Soldier_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Giant_Soldier_L.bmp", L"Giant_Soldier_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Mage_L.bmp", L"Mage_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Enemy/Mage_R.bmp", L"Mage_R");

	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------투사체---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Slash_EFFECT_L.bmp", L"Slash_EFFECT_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Slash_EFFECT_R.bmp", L"Slash_EFFECT_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Meteor_Bullet_R.bmp", L"Meteor_Bullet_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Meteor_Bullet_L.bmp", L"Meteor_Bullet_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Golden_Arrow_R.bmp", L"Golden_Arrow_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Golden_Arrow_L.bmp", L"Golden_Arrow_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/Mage_Core.bmp", L"Mage_Core");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Projectile/EntSkul_Fiest_Effect.bmp", L"EntSkul_Fiest_Effect");

	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------이펙트---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Ground_Effect.bmp", L"BossC_Ground_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Judgement_Effect.bmp", L"BossC_Judgement_Effect");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Laser_Effect.bmp", L"BossC_Laser_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Laser_Core.bmp", L"BossC_Laser_Core");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Meteor_Effect.bmp", L"BossC_Meteor_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Ground_Sign.bmp", L"Ground_Sign");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Awaken_effect_Flame.bmp", L"Boss_Awaken_effect_Flame");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Awaken_effect_Elect.bmp", L"Boss_Awaken_effect_Elect");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Player_Jump_Effect.bmp", L"Player_Jump_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Player_Dash_Smoke_L.bmp", L"Player_Dash_Smoke_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Player_Dash_Smoke_R.bmp", L"Player_Dash_Smoke_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Skul_Atk_Effect_L.bmp", L"Skul_Atk_Effect_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Skul_Atk_Effect_R.bmp", L"Skul_Atk_Effect_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Monster_Atk_Effect.bmp", L"Monster_Atk_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Move_Skul_Effect.bmp", L"Move_Skul_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Ground_Smoke.bmp", L"Boss_Ground_Smoke");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_MeteorSmoke_L.bmp", L"Boss_MeteorSmoke_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_MeteorSmoke_R.bmp", L"Boss_MeteorSmoke_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Arrow_Sign.bmp", L"Arrow_Sign");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Tackle_Effect_R.bmp", L"Tackle_Effect_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Tackle_Effect_L.bmp", L"Tackle_Effect_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Giant_Impact.bmp", L"Giant_Impact");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/EntSkul_Grab_Effect.bmp", L"EntSkul_Grab_Effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/EntSkul_Swap_Effect.bmp", L"EntSkul_Swap_Effect");
	
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Wolf_SkillA_Effect_L.bmp", L"Wolf_SkillA_Effect_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Wolf_SkillA_Effect_R.bmp", L"Wolf_SkillA_Effect_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Wolf_SkillB_Effect_L.bmp", L"Wolf_SkillB_Effect_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Wolf_SkillB_Effect_R.bmp", L"Wolf_SkillB_Effect_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Enhance_MeteorSmoke_R.bmp", L"Boss_Enhance_MeteorSmoke_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Boss_Enhance_MeteorSmoke_L.bmp", L"Boss_Enhance_MeteorSmoke_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BossC_Dead_effect.bmp", L"BossC_Dead_effect");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Homing_APPEAR_L.bmp", L"Homing_APPEAR_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Homing_APPEAR_R.bmp", L"Homing_APPEAR_R");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Dark_meteor_Sign.bmp", L"Dark_meteor_Sign");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Dark_meteor_Impact.bmp", L"Dark_meteor_Impact");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Dark_meteor_Ground.bmp", L"Dark_meteor_Ground");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BigDash_Smoke_L.bmp", L"BigDash_Smoke_L");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BigDash_Smoke_R.bmp", L"BigDash_Smoke_R");

	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Legend_Behind.bmp", L"Legend_Behind");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Legend_Front.bmp", L"Legend_Front");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Golden_Meteor_Effect.bmp", L"Golden_Meteor_Effect");

	/*----------------------------------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------아이템---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Wolf_Skul_Item.bmp", L"Wolf_Skul_Item");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Ent_Skul_Item.bmp", L"Ent_Skul_Item");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Stage_Box.bmp", L"Stage_Box");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Jusin.bmp", L"Jusin");
	/*------------------------------------------------------NPC---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/NPC_Orge.bmp", L"NPC_Orge");

	/*------------------------------------------------------파티클---------------------------------------------------------------*/
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_1.bmp", L"Parts_1.");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_2.bmp",L"Parts_2");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_3.bmp",L"Parts_3");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_4.bmp",L"Parts_4");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_5.bmp",L"Parts_5");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_6.bmp",L"Parts_6");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_7.bmp",L"Parts_7");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_8.bmp",L"Parts_8");
	CResourceMgr::Get_Instance()->Insert_Bmp(L"../Image/Particle/Parts_9.bmp",L"Parts_9");

}
