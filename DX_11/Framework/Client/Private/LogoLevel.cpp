#include "Client_Defines.h"
#include "LogoLevel.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "ILevelService.h"

#include "GamePlayLevel.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IResourceService.h"
#include "IFontService.h"

#include "ClientHelper.h"
#include "Builder.h"

#include "Player.h"
#include "NonPlayer.h"
#include "NpcRco.h"
#include "NpcRcm.h"
#include "NpcTkk.h"
#include "NpcJcs.h"

#include "Target_Camera.h"
#include "Free_Camera.h"
#include "SunCam.h"
#include "Camera.h"
#include "Transform.h"

#include "ToolItem.h"
#include "HairParts.h"
#include "HairCapParts.h"
#include "ClothParts.h"
#include "Glass_Acc.h"
#include "PlayerPart_Hand.h"
#include "MapLoader.h"
#include "AutoTile.h"
#include "Layer.h"
#include "FieldHole.h"

#include "Player_Inventory.h"
#include "Item_Object.h"
#include "FishSub_Tool.h"

#include "UI_InvenSlot.h"
#include "UI_Cursor.h"
#include "UI_ItemIcon.h"
#include "UI_ItemText.h"
#include "TexturePanel.h"
#include "SelectPanel.h"
#include "CraftCard.h"
#include "UI_Text.h"
#include "UI_EventMsg.h"
#include "UI_NameTag.h"
#include "UI_TalkingMsg.h"
#include "UI_CraftPanel.h"
#include "UI_ItemCard.h"
#include "ScreenFX.h"

#include "Target_Texture.h"
#include "Target_Text.h"

#include "Insect_Object.h"
#include "Fish_Object.h"

#include "FishSpawner.h"
#include "ItemSpawner.h"
#include "InsectSpawner.h"
#include "UI_Responcer.h"
#include "EventSystem.h"
#include "UI_Transition.h"
#include "NpcSpawner.h"
#include "DialogueManager.h"
#include "EffectSpawner.h"
#include "SkyBox.h"
#include "DustEffect.h"
#include "Cloud_Env.h"
#include "Cloud_Space.h"
#include "LeafParticle.h"
#include "UI_Object.h"
#include "EndingController.h"
#include "SceneCamera.h"
#include "Moon.h"
#include "Stars.h"

CLogoLevel::CLogoLevel(const string& LevelKey)
	: CLevel{ LevelKey },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLogoLevel::Initialize()
{
	
	return S_OK;
}

HRESULT CLogoLevel::Awake()
{
	return S_OK;
}

void CLogoLevel::Update()
{
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_SPACE)) {
		CGameInstance::GetInstance()->Get_LevelMgr()->Request_ChangeLevel("GamePlay_Level");
	}
}

HRESULT CLogoLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
	return S_OK;
}

CLogoLevel* CLogoLevel::Create(const string& LevelKey)
{
	CLogoLevel* instance = new CLogoLevel(LevelKey);
	if (FAILED(instance->Initialize())) {
		MSG_BOX("LOGO level Create Failed");
		Safe_Release(instance);
	}

	return instance;
}

void CLogoLevel::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}

void CLogoLevel::PreLoad_Level()
{

}
