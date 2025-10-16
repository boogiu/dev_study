#include "Client_Defines.h"
#include "GamePlayLevel.h"
#include "GameInstance.h"
#include "ILevelService.h"
#include "IProtoService.h"
#include "IObjectService.h"
#include "IResourceService.h"
#include "IRenderService.h"

#include "Builder.h"
#include "Player.h"
#include "Target_Camera.h"
#include "Free_Camera.h"
#include "ICameraService.h"
#include "Camera.h"

#include "MapLoader.h"
#include "ClientHelper.h"

#include "ITileService.h"

CGamePlayLevel::CGamePlayLevel(const string& LevelKey)
    :CLevel{ LevelKey },
    m_pGameInstance(CGameInstance::GetInstance())
{

    Safe_AddRef(m_pGameInstance);
    m_pProtoManager = m_pGameInstance->Get_PrototypeMgr();
    m_pObjectManager = m_pGameInstance->Get_ObjectMgr();

}

HRESULT CGamePlayLevel::Initialize()
{
    //m_pGameInstance->Get_TileSystem()->

    CGameObject* pPlayer = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_Player" }).Position({ 550,0,550 }).Build("Player");

    CTarget_Camera::TARGET_CAM_DESC* pCamDesc = new CTarget_Camera::TARGET_CAM_DESC;
    pCamDesc->pTarget = pPlayer;

    CGameObject* pCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_TargetCamera" })
        .Camera({ (float)Client::g_iWinSizeX / Client::g_iWinSizeY })
        .Add_ObjDesc(pCamDesc)
        .Build("Main_Cam");

    CGameObject* pFreeCamera = Builder::Create_Object({ "GamePlay_Level","GamePlay_GameObject_FreeCamera" })
        .Camera({ (float)Client::g_iWinSizeX / Client::g_iWinSizeY })
        .Build("Free_Cam");

    m_pObjectManager->Add_Object(pPlayer, { "GamePlay_Level", "Player_Layer" });
    m_pObjectManager->Add_Object(pCamera, { "GamePlay_Level", "Camera_Layer" });
    m_pObjectManager->Add_Object(pFreeCamera, { "GamePlay_Level", "Camera_Layer" });

    CGameInstance::GetInstance()->Get_CameraMgr()->Set_MainCam(pFreeCamera->Get_Component<CCamera>());
    return S_OK;
}

void CGamePlayLevel::Update()
{
}

HRESULT CGamePlayLevel::Render()
{
    return S_OK;
}

HRESULT CGamePlayLevel::Render(ID3D11DeviceContext* pContext)
{
    SetWindowText(g_hWnd, TEXT("GamePlayLevel."));

        auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();

        if (TileSystem)
            TileSystem->Render_Tiles(pContext);

    return S_OK;
}

void CGamePlayLevel::PreLoad_Level()
{
    auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
    auto pRenderSys = CGameInstance::GetInstance()->Get_RenderSystem();

    /*Add Palette*/
    pRcsMgr->Add_ResourcePath("mGrass_Grd.dds", "../../Resources/Palette/mGrass_Grd.dds");
    pRcsMgr->Add_ResourcePath("mGrass_GrdEdge.dds", "../../Resources/Palette/mGrass_GrdEdge.dds");
    pRcsMgr->Add_ResourcePath("mGrass_Mix.dds", "../../Resources/Palette/mGrass_Mix.dds");

    pRenderSys->Add_Palette("g_PaletteTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Grd.dds"));
    pRenderSys->Add_Palette("g_PaletteEdgeTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_GrdEdge.dds"));
    pRenderSys->Add_Palette("g_MaskTexture", pRcsMgr->Load_Texture(G_GlobalLevelKey, "mGrass_Mix.dds"));

    /*Player Model Path*/
    pRcsMgr->Add_ResourcePath("PlayerBody.model", "../../Resources/Models/Player/PlayerBody.model");
    pRcsMgr->Add_ResourcePath("PlayerBody.mat", "../../Resources/Models/Player/PlayerBody.mat");

    /*Tiles  Path*/
    ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/FieldRoad");
    ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/FieldRoad");

    /*FieldOuts  Path*/
    ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/FieldOut");
    ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/FieldOut");  

    /*Structure  Path*/
    ClientHelper::Add_ModelPathFromDirectory("../../Resources/Models/Structure");
    ClientHelper::Add_MaterialPathFromDirectory("../../Resources/Models/Structure");
    ClientHelper::Add_AnimPathFromDirectory("../../Resources/Models/Player/Animations/Movement");

    /*Object_Prototype*/
    auto pProtoMgr = CGameInstance::GetInstance()->Get_PrototypeMgr();
    pProtoMgr->Add_ProtoType("GamePlay_Level","GamePlay_GameObject_Player",CPlayer::Create());
    pProtoMgr->Add_ProtoType("GamePlay_Level","GamePlay_GameObject_TargetCamera",CTarget_Camera::Create());
    pProtoMgr->Add_ProtoType("GamePlay_Level","GamePlay_GameObject_FreeCamera",CFree_Camera::Create());

    CMapLoader::Load_MapData("../../Resources/Data/MapData.dat", { "GamePlay_Level", "Field_Layer" });
 }

CGamePlayLevel* CGamePlayLevel::Create(const string& LevelKey)
{
    CGamePlayLevel* instance = new CGamePlayLevel(LevelKey);
    if (FAILED(instance->Initialize())) {
        MSG_BOX("GamePlay level Create Failed");
        Safe_Release(instance);
    }
    return instance;
}

void CGamePlayLevel::Free()
{
    __super::Free();
    Safe_Release(m_pGameInstance);
}
